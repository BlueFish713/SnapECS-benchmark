#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <vector>

#include <SnapECS/Core.hpp>
#include <SnapECS/detail/CachedBuffers.hpp>
#include <SnapECS/detail/Meta.hpp>
#include <SnapECS/detail/RowAccessor.hpp>

namespace snapecs::detail {

// SystemViewTable 안에서 Entity의 상태 enum
// 구조 변경은 PendingAdd/PendingRemove로 둔 뒤, 다음 순회에서 병합
enum class ViewEntityState : std::uint8_t {
  Absent,
  Active,
  PendingAdd,
  PendingRemove
};

template <typename Signature, typename ComponentList>
class SystemViewTable;

// SystemViewTable
template <typename... Slots, typename ComponentList>
class SystemViewTable<AccessSignature<Slots...>, ComponentList> {
public:
  using Signature = AccessSignature<Slots...>;
  using CachedComponents = SignatureCachedComponentsT<Signature>;
  using WrittenCachedComponents = SignatureWrittenCachedComponentsT<Signature>;

  // 연관된
  template <typename Component>
  static constexpr bool RelatedTo = ((std::is_same_v<typename Slots::ComponentType, Component> &&
                                      Slots::Related) ||
                                     ...);

  // Cached
  template <typename Component>
  static constexpr bool StoresCached = ((std::is_same_v<typename Slots::ComponentType, Component> &&
                                         Slots::StoresCached) ||
                                        ...);
  // Indexed
  template <typename Component>
  static constexpr bool StoresIndexed = ((std::is_same_v<typename Slots::ComponentType, Component> &&
                                          Slots::StoresIndexed) ||
                                         ...);
  // Read
  template <typename Component>
  static constexpr bool CanRead = ((std::is_same_v<typename Slots::ComponentType, Component> &&
                                    Slots::Reads) ||
                                   ...);
  // Write
  template <typename Component>
  static constexpr bool CanWrite = ((std::is_same_v<typename Slots::ComponentType, Component> &&
                                     Slots::Writes) ||
                                    ...);

  // cached component 가져오기(ref)                                  
  template <typename Component>
  Component &cached_at(bool oddBuffer, std::size_t row) {
    return cached_.template at<Component>(oddBuffer, row);
  }

  // cached component 가져오기(const ref) 
  template <typename Component>
  const Component &cached_at(bool oddBuffer, std::size_t row) const {
    return cached_.template at<Component>(oddBuffer, row);
  }

  // 엔티티 개수
  std::size_t entity_count() const {
    return entityCount_;
  }

  // entity 존재 여부
  bool contains(Entity entity) const {
    const auto state = state_of(entity);
    return state == ViewEntityState::Active || state == ViewEntityState::PendingAdd;
  }

  // entity 추가
  void enqueue_add(Entity entity) {
    ensure_entity(entity);
    const auto state = state_[entity];

    if (state == ViewEntityState::Active || state == ViewEntityState::PendingAdd) {
      return;
    }

    if (state == ViewEntityState::PendingRemove) {
      // remove 예정이었으면 Active 처리
      state_[entity] = ViewEntityState::Active;
      return;
    }

    // pendingAdd에 추가
    state_[entity] = ViewEntityState::PendingAdd;
    push_pending(pendingAdds_, pendingAddCount_, entity);
  }

  // entity 삭제
  void enqueue_remove(Entity entity) {
    ensure_entity(entity);
    const auto state = state_[entity];

    if (state == ViewEntityState::PendingAdd) {
      // add 예정이었으면 Absent 처리
      state_[entity] = ViewEntityState::Absent;
      return;
    }

    // entity가 존재하면
    if (state == ViewEntityState::Active) {
      state_[entity] = ViewEntityState::PendingRemove;
      // pendingRemove에 추가
      push_pending(pendingRemoves_, pendingRemoveCount_, entity);
    }
  }

  template <typename WorldT>
  bool matches(const WorldT &world, Entity entity) const {
    //active 이면서  AccessSlot 조건 만족
    return world.is_active(entity) && ((matches_slot<Slots>(world, entity)) && ...);
  }

  template <typename Component>
  void update_cached_copy_if_present(Entity entity, const Component &value) {
    if constexpr (StoresCached<Component>) {
      const auto state = state_of(entity);
      if (state == ViewEntityState::Active || state == ViewEntityState::PendingRemove) {
        // PendingRemove도 아직 active buffer에 row가 남아 있으므로 동기화 대상입니다.
        cached_.template at<Component>(oddActive_, rowIndex_[entity]) = value;
      }
    }
  }

  // 순회하는 함수
  template <typename SystemT, typename WorldT>
  void for_each(WorldT &world, SystemT &system) {
    // Begin Update() 함수 호출
    call_begin_update(system);

    // 구조 변경을 위한 World Context 생성
    auto worldContext = make_world_context<SystemT>(world);

    // 이 시점의 pending add/remove 개수를 snapshot으로 보존
    auto snapshot = prepare_pending_snapshot();

    // pending 변경이 없을 때
    if (snapshot.addCount == 0 && snapshot.removeCount == 0) {
      const bool activeOdd = oddActive_;
      auto &entities = entity_buffer(activeOdd);

      // 순회 실행
      for (std::size_t row = 0; row < entityCount_; ++row) {
        invoke_row<SystemT>(world, system, worldContext, entities[row], activeOdd, row);
      }

      finish_pending_snapshot(snapshot);  //pendingBuffer 초기화
      
      // End Update() 함수 호출
      call_end_update(system);
      return;
    }

    // pending 변경이 있으면 source buffer -> target buffer 이동
    const bool sourceOdd = oddActive_; //source buffer가 odd인지
    const bool targetOdd = !oddActive_; //target buffer가 odd인지
    const auto plannedEntityCount = entityCount_ + snapshot.addCount - snapshot.removeCount; // target buffer로 이동될 enitity

    // entity 가져오기
    auto &sourceEntities = entity_buffer(sourceOdd);
    auto &targetEntities = entity_buffer(targetOdd);

    // target 버퍼 크기 resize
    EnsureBufferSize(targetEntities, plannedEntityCount);
    cached_.ensure_size(targetOdd, plannedEntityCount); //cached 저장용

    std::size_t sourceIndex = 0;
    std::size_t addIndex = 0;
    std::size_t removeIndex = 0;
    std::size_t writeIndex = 0;

    // 캡처 리스트로 참조
    // pendingAdd의 요소
    auto next_add = [&]() -> Entity {
      return addIndex < snapshot.addCount ? pendingAdds_[addIndex]
                                          : std::numeric_limits<Entity>::max(); //max로 반환 시 무조건 비교값보다 큼
    };

    // 원본 buffer의 요소
    auto next_source = [&]() -> Entity {
      return sourceIndex < entityCount_ ? sourceEntities[sourceIndex]
                                        : std::numeric_limits<Entity>::max();
    };

    // remove해야하는지 확인
    auto should_remove = [&](Entity entity) {
      while (removeIndex < snapshot.removeCount && pendingRemoves_[removeIndex] < entity) {
        ++removeIndex;
      }
      return removeIndex < snapshot.removeCount && pendingRemoves_[removeIndex] == entity;
    };

    // 기존 active entity 목록과 정렬된 pending add 목록을 entity ID 오름차순으로 병합합니다.
    while (sourceIndex < entityCount_ || addIndex < snapshot.addCount) {
      if (next_add() < next_source()) { // 다음 값보다 작으면 add
        const Entity entity = pendingAdds_[addIndex++];
        if (state_of(entity) != ViewEntityState::PendingAdd) { // 혹시 몰라서 state 조회
          continue;
        }

        targetEntities[writeIndex] = entity;
        cached_.copy_from_source(targetOdd, writeIndex, world, entity);
        activate_row(entity, writeIndex); // 추가된 entity의 row 생성
        invoke_row<SystemT>(world, system, worldContext, entity, targetOdd, writeIndex); // 해당 row 실행(Update 함수 실행)
        ++writeIndex;
        continue;
      }

      const Entity entity = sourceEntities[sourceIndex]; //add가 아니면 원본 buffer에서 가져옴
      const std::size_t oldRow = sourceIndex++; // 이전 index 저장 후 증가

      if (should_remove(entity)) { //삭제해야하면 삭제 처리
        state_[entity] = ViewEntityState::Absent;
        continue;
      }

      // 삭제 대상이 아니면 이동
      targetEntities[writeIndex] = entity;
      // cached 이동
      cached_.copy_between(sourceOdd, oldRow, targetOdd, writeIndex);
      activate_row(entity, writeIndex); // 추가된 entity의 row 생성
      invoke_row<SystemT>(world, system, worldContext, entity, targetOdd, writeIndex); // 해당 row 실행(Update 함수 실행)
      ++writeIndex;
    }

    assert(writeIndex == plannedEntityCount); // 확인

    entityCount_ = plannedEntityCount; //entityCount_ 업데이트
    oddActive_ = !oddActive_; //odd 전환

    finish_pending_snapshot(snapshot); //pendingBuffer 초기화
    
    // End Update() 함수 호출
    call_end_update(system);
  }

/////////////////////////////////////////////////////////////////////////////////

private:
  struct NoWorldContext {};

  struct PendingSnapshot {
    std::size_t addCount = 0;
    std::size_t removeCount = 0;
  };

  template <typename SystemT>
  static void call_begin_update(SystemT &system) {
    if constexpr (requires(SystemT &value) { value.BeginUpdate(); }) {
      system.BeginUpdate();
    }
  }

  template <typename SystemT>
  static void call_end_update(SystemT &system) {
    if constexpr (requires(SystemT &value) { value.EndUpdate(); }) {
      system.EndUpdate();
    }
  }

  // 구조 변경을 위한 WorldContext 생성
  template <typename SystemT, typename WorldT>
  static auto make_world_context(WorldT& world) {
    if constexpr (std::is_same_v<typename SystemT::StructuralChangeType,
                                 NoStructuralChange>) {
      return NoWorldContext{};
    } else {
      return typename SystemT::WorldContext{world};
    }
  }

  // Access Slot과 Entity가 매치하는지 확인
  template <typename Slot, typename WorldT>
  static bool matches_slot(const WorldT &world, Entity entity) {
    using Component = typename Slot::ComponentType;

    if constexpr (Slot::AdmissionRequired && Slot::EvictionRequired) {
      // Admission 이면서 Eviction이면 false 반환
      return false;
    } else if constexpr (Slot::AdmissionRequired) {
      return world.template has_component<Component>(entity);
    } else if constexpr (Slot::EvictionRequired) {
      return !world.template has_component<Component>(entity);
    } else {
      return true;
    }
  }

  static void push_pending(std::vector<Entity> &buffer, std::size_t &count, Entity entity) {
    if (buffer.size() <= count) {
      buffer.push_back(entity);
    } else {
      buffer[count] = entity;
    }
    ++count;
  }

  // state_ 배열의 크기 보장
  void ensure_entity(Entity entity) {
    const auto required = static_cast<std::size_t>(entity) + 1;
    if (state_.size() < required) {
      state_.resize(required, ViewEntityState::Absent);
      rowIndex_.resize(required, 0);
    }
  }

  // Entity의 State 반환
  ViewEntityState state_of(Entity entity) const {
    const auto index = static_cast<std::size_t>(entity);
    return index < state_.size() ? state_[index] : ViewEntityState::Absent;
  }
  
  void activate_row(Entity entity, std::size_t row) {
    ensure_entity(entity);

    state_[entity] = ViewEntityState::Active;
    rowIndex_[entity] = row;
  }

  // pending structural change 목록을 정렬하고 중복 제거합니다.
  // Entity가 unsigned int라 byte 단위 radix sort를 고정 pass로 수행할 수 있습니다.
  static void sort_unique_pending(std::vector<Entity> &pending, std::size_t &count) {
    if (count < 2) {
      return;
    }

    constexpr std::size_t BucketCount = 256;
    std::vector<Entity> scratch(count);

    for (std::size_t shift = 0; shift < sizeof(Entity) * 8; shift += 8) {
      std::array<std::size_t, BucketCount> offsets{};

      for (std::size_t index = 0; index < count; ++index) {
        ++offsets[(pending[index] >> shift) & 0xffU];
      }

      std::size_t writeOffset = 0;
      for (std::size_t bucket = 0; bucket < BucketCount; ++bucket) {
        const auto bucketSize = offsets[bucket];
        offsets[bucket] = writeOffset;
        writeOffset += bucketSize;
      }

      for (std::size_t index = 0; index < count; ++index) {
        const auto bucket = (pending[index] >> shift) & 0xffU;
        scratch[offsets[bucket]++] = pending[index];
      }

      for (std::size_t index = 0; index < count; ++index) {
        pending[index] = scratch[index];
      }
    }

    std::size_t uniqueCount = 1;
    for (std::size_t index = 1; index < count; ++index) {
      if (pending[index] != pending[uniqueCount - 1]) {
        pending[uniqueCount++] = pending[index];
      }
    }
    count = uniqueCount;
  }

  static std::size_t compact_pending(std::vector<Entity> &pending,
                                     std::size_t count,
                                     const std::vector<ViewEntityState> &state,
                                     ViewEntityState wantedState) {
    std::size_t validCount = 0;

    // 중간 상태 전이로 무효화된 pending 항목은 버립니다.
    for (std::size_t index = 0; index < count; ++index) {
      const Entity entity = pending[index];
      if (static_cast<std::size_t>(entity) < state.size() && state[entity] == wantedState) {
        pending[validCount++] = entity;
      }
    }

    sort_unique_pending(pending, validCount);
    return validCount;
  }

  PendingSnapshot prepare_pending_snapshot() {
    pendingAddCount_ =
        compact_pending(pendingAdds_, pendingAddCount_, state_, ViewEntityState::PendingAdd);
    pendingRemoveCount_ = compact_pending(
        pendingRemoves_, pendingRemoveCount_, state_, ViewEntityState::PendingRemove);
    return PendingSnapshot{pendingAddCount_, pendingRemoveCount_};
  }

  void finish_pending_snapshot(PendingSnapshot snapshot) {
    finish_pending_buffer(pendingAdds_, pendingAddCount_, snapshot.addCount);
    finish_pending_buffer(pendingRemoves_, pendingRemoveCount_, snapshot.removeCount);
  }

  static void finish_pending_buffer(std::vector<Entity> &pending,
                                    std::size_t &count,
                                    std::size_t processedCount) {
    // 처리한 snapshot 구간만 제거
    const auto remainingCount = count - processedCount;
    for (std::size_t index = 0; index < remainingCount; ++index) {
      pending[index] = pending[processedCount + index];
    }
    count = remainingCount;
  }

  std::vector<Entity> &entity_buffer(bool oddBuffer) {
    return oddBuffer ? oddEntities_ : evenEntities_;
  }

  const std::vector<Entity> &entity_buffer(bool oddBuffer) const {
    return oddBuffer ? oddEntities_ : evenEntities_;
  }

  template <typename SystemT, typename WorldT, typename WorldContextT>
  void invoke_row(WorldT &world,
                  SystemT &system,
                  WorldContextT& worldContext,
                  Entity entity,
                  bool bufferOdd,
                  std::size_t row) {
    RowAccessor<SystemViewTable, WorldT> accessor(*this, world, entity, bufferOdd, row); //entity row 생성
    invoke_update<SystemT>(system,
                           worldContext,
                           accessor,
                           entity,
                           typename SystemT::EntityReadOnlyList{},
                           typename SystemT::EntityWriteList{});

    sync_written_cached(world, entity, bufferOdd, row);
  }

  // 사용자가 구현한 Update() 실행
  template <typename SystemT,
            typename WorldContextT,
            typename AccessorT,
            typename... ReadComponents,
            typename... WriteComponents>
  static void invoke_update(SystemT &system,
                            WorldContextT& worldContext,
                            AccessorT &accessor,
                            Entity entity,
                            mp::mp_list<ReadComponents...>,
                            mp::mp_list<WriteComponents...>) {
    using BaseSystem =
        System<typename SystemT::QueryType,
               typename SystemT::ReadType,
               typename SystemT::WriteType,
               typename SystemT::StructuralChangeType>;

    if constexpr (std::is_same_v<typename SystemT::StructuralChangeType,
                                 NoStructuralChange>) {

      // Update()가 순수 가상 함수이기에 사용자의 구현으로 자동 디스패치
      static_cast<BaseSystem &>(system).Update(
          entity,
          accessor.template read<ReadComponents>()...,
          accessor.template write<WriteComponents>()...);
    } else {
      static_cast<BaseSystem &>(system).Update(
          worldContext,
          entity,
          accessor.template read<ReadComponents>()...,
          accessor.template write<WriteComponents>()...);
    }
  }

  template <typename WorldT>
  void sync_written_cached(WorldT &world, Entity entity, bool bufferOdd, std::size_t row) {
    sync_written_cached_impl(world, entity, bufferOdd, row, WrittenCachedComponents{});
  }

  template <typename WorldT, typename... Components>
  void sync_written_cached_impl(WorldT &world,
                                Entity entity,
                                bool bufferOdd,
                                std::size_t row,
                                mp::mp_list<Components...>) {
    // 최신 값과 source table과 다른 cached view로 동기화
    ((world.template sync_cached_component<Components>(
         entity, cached_.template at<Components>(bufferOdd, row), static_cast<void *>(this))), // 타입이 다른 경우를 커버하기 위해 void 포인터로 전달
     ...);
  }

  std::vector<Entity> oddEntities_;
  std::vector<Entity> evenEntities_;
  bool oddActive_ = true;
  std::size_t entityCount_ = 0;

  CachedBuffers<CachedComponents> cached_;

  std::vector<ViewEntityState> state_;
  std::vector<std::size_t> rowIndex_;

  std::vector<Entity> pendingAdds_;
  std::vector<Entity> pendingRemoves_;
  std::size_t pendingAddCount_ = 0;
  std::size_t pendingRemoveCount_ = 0;
};

} // namespace snapecs::detail