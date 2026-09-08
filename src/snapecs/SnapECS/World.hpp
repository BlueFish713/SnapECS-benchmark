#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#include <SnapECS/Core.hpp>
#include <SnapECS/detail/ComponentSourceTable.hpp>
#include <SnapECS/detail/EntityRegistry.hpp>
#include <SnapECS/detail/Meta.hpp>
#include <SnapECS/detail/ThreadPool.hpp>
#include <SnapECS/detail/ViewDispatcher.hpp>

namespace snapecs {

// World
template <typename ComponentsT, typename SystemsT>
class World;

template <typename... RegisteredComponents, typename... RegisteredUnits>
class World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>> {
public:
  // Components<...> 그룹을 재귀적으로 펼친 실제 컴포넌트 등록 목록입니다.
  using ComponentList =
      detail::FlattenComponentsT<mp::mp_list<RegisteredComponents...>>;
  using RegisteredUnitList = mp::mp_list<RegisteredUnits...>;

  // Systems 내부 Unit을 flatten하여 SystemList로 저장
  using SystemList = detail::FlattenUnitsT<RegisteredUnitList>;

  // AccessSignature: {전체 View} 대상
  // SystemList의 System들의 AccessSignature를 저장
  using SystemSignatureList =
      mp::mp_transform<detail::MakeAccessSignatureFor<ComponentList>::template Fn, SystemList>;

  // ViewSignature: {전체 View} - {Single Component} => (entity view signature에서는 제외)
  // SystemList의 System들의 ViewSignature를 저장
  using SystemViewSignatureList =
      mp::mp_transform<detail::MakeViewSignatureFor<ComponentList>::template Fn, SystemList>;

  // 같은 view signature를 가진 system들이 view table을 공유하도록 SystemViewSignatureList의 중복 제거
  using ViewSignatureList = mp::mp_unique<SystemViewSignatureList>;

  static_assert(detail::ComponentSetIsUnique<ComponentList>::value,
                "Components<...> must not contain duplicates.");
  static_assert(detail::TypeListIsUnique<SystemList>::value,
                "Each System type can be registered only once in the scheduler tree.");

  // 스케줄러 mask bit 관련 상수
  static constexpr std::size_t ComponentCount = mp::mp_size<ComponentList>::value;
  static constexpr std::size_t SystemCount = mp::mp_size<SystemList>::value;
  static constexpr std::size_t ViewSignatureCount = mp::mp_size<ViewSignatureList>::value;
  static constexpr std::size_t SchedulerAccessBitCount = ComponentCount + ViewSignatureCount + 1;
  static constexpr std::size_t SchedulerMaskWordCount = (SchedulerAccessBitCount + 63) / 64;
  static constexpr std::size_t EntityRegistryAccessBit = ComponentCount + ViewSignatureCount;

  using SchedulerMask = detail::AccessMask<SchedulerMaskWordCount>;

  template <typename Component>
  static constexpr std::size_t ComponentId = mp::mp_find<ComponentList, Component>::value;

  template <typename SystemT>
  static constexpr std::size_t SystemId = mp::mp_find<SystemList, SystemT>::value;

  template <typename SystemT>
  using SystemSignature = detail::MakeAccessSignatureT<ComponentList, SystemT>;

  template <typename SystemT>
  using SystemViewSignature = detail::MakeViewSignatureT<ComponentList, SystemT>;

  template <typename SystemT>
  static constexpr std::size_t ViewId =
      mp::mp_find<ViewSignatureList, SystemViewSignature<SystemT>>::value;

  template <typename SystemT>
  static constexpr SchedulerMask system_access_mask();

  // 두 system이 read/write 충돌을 일으키는지 확인
  template <typename SystemA, typename SystemB>
  static constexpr bool SystemsConflict =
      detail::masks_conflict(system_access_mask<SystemA>(), system_access_mask<SystemB>());

  World();
  explicit World(SchedulerConfig config);

  // update
  void update();

  void configure_scheduler(SchedulerConfig config);
  const SchedulerConfig& scheduler_config() const;

  GroupId main_group() const { return GroupId{MainGroupIndex}; }
  GroupId input_group() const { return GroupId{InputGroupIndex}; }
  GroupId physics_group() const { return GroupId{PhysicsGroupIndex}; }
  GroupId script_group() const { return GroupId{ScriptGroupIndex}; }
  GroupId render_group() const { return GroupId{RenderGroupIndex}; }

  GroupId create_group(GroupId parent,
                       GroupPolicy policy = GroupPolicy::Parallel,
                       SchedulePosition position = SchedulePosition::Auto);
  bool destroy_group(GroupId group);
  bool move_group(GroupId group, GroupId newParent);

  template <typename SystemT>
  bool insert_system(GroupId parent, SchedulePosition position = SchedulePosition::Auto);

  template <typename SystemT>
  bool remove_system();

  template <typename SystemT>
  bool move_system(GroupId newParent);

  Entity create_entity(bool active = true);
  bool is_active(Entity entity) const;
  void activate_entity(Entity entity);
  void deactivate_entity(Entity entity);

  template <typename Component, typename... Args>
  void add_component(Entity entity, Args&&... args);

  template <typename Component>
  void remove_component(Entity entity);

  template <typename Component>
  bool has_component(Entity entity) const;

  template <typename Component>
  Component& get_component(Entity entity);

  template <typename Component>
  const Component& get_component(Entity entity) const;

  template <typename Component, typename... Args>
  void set_component(Entity entity, Args&&... args);

  template <typename Component, typename Fn>
  void update_component(Entity entity, Fn&& fn);

  template <typename Component>
  Component& single_component();

  template <typename Component>
  const Component& single_component() const;

  template <typename SystemT>
  SystemT& system();

  template <typename SystemT>
  const SystemT& system() const;

  template <typename SystemT>
  auto& view_for();

  template <typename SystemT>
  const auto& view_for() const;

  template <typename Component>
  auto& source_table();

  template <typename Component>
  const auto& source_table() const;

  template <typename Component>
  void sync_cached_component(Entity entity, const Component& value, void* skipView);

private:
  static constexpr std::size_t Npos = static_cast<std::size_t>(-1);
  static constexpr std::size_t MainGroupIndex = 0;
  static constexpr std::size_t InputGroupIndex = 1;
  static constexpr std::size_t PhysicsGroupIndex = 2;
  static constexpr std::size_t ScriptGroupIndex = 3;
  static constexpr std::size_t RenderGroupIndex = 4;

  enum class UnitKind {
    System,
    Group
  };

  // system에 대한 런타임 메타데이터
  struct RuntimeSystemInfo {
    SchedulerMask mask{};
    void (*run)(World&) = nullptr;
    std::size_t (*work)(const World&) = nullptr;
  };

  // group child linked list의 노드
  struct UnitEntry {
    bool active = true;
    UnitKind kind = UnitKind::System;
    std::size_t id = Npos;
    std::size_t parentGroup = Npos;
    std::size_t prev = Npos;
    std::size_t next = Npos;
    SchedulePosition position = SchedulePosition::Auto;
  };

  // Parallel group은 before -> batches -> after 순서로 실행
  struct GroupPlan {
    std::vector<std::size_t> beforeEntries;
    std::vector<std::vector<std::size_t>> batches;
    std::vector<std::size_t> afterEntries;
  };

  // update에서 실제로 실행하는 평탄화된 실행 계획입니다.
  // group entry는 여기에 들어오지 않고 leaf system entry만 들어옵니다.
  struct ExecutionStep {
    std::vector<std::size_t> systemEntries;
  };

  struct ExecutionPlan {
    std::vector<ExecutionStep> steps;
  };

  struct GroupNode {
    bool active = true;
    bool fixed = false;
    bool dirty = true;
    GroupPolicy policy = GroupPolicy::Parallel;
    std::size_t parentEntry = Npos;
    std::size_t firstChild = Npos;
    std::size_t lastChild = Npos;
    SchedulerMask mask{};
    GroupPlan plan;
  };

  using SourceTables = detail::SourceTableTupleT<ComponentList>;
  using ViewTables = detail::ViewTableTupleT<ViewSignatureList, ComponentList>;  // View Signature와 Component의 Table 생성

  // Singleton 참조를 생성자에서 받는 system은 기본 생성할 수 없으므로 optional에 지연 생성합니다.
  template <typename SystemT>
  using SystemStorage = std::optional<SystemT>;

  using SystemInstances =
      mp::mp_rename<mp::mp_transform<SystemStorage, SystemList>, std::tuple>;

  template <typename Component>
  using RelatedViewIndices = detail::RelatedViewIndicesT<Component, ViewSignatureList>;

  template <typename Component>
  using CachedSyncViewIndices = detail::CachedSyncViewIndicesT<Component, ViewSignatureList>;

  template <typename SystemT>
  static constexpr SchedulerMask make_system_mask();

  template <typename SystemT, typename... Slots>
  static constexpr void add_signature_bits(SchedulerMask& mask, detail::AccessSignature<Slots...>);

  template <typename Slot>
  static constexpr void add_slot_bits(SchedulerMask& mask);

  template <typename SystemT>
  static constexpr void add_structural_bits(SchedulerMask& mask);

  template <typename... ComponentsT>
  static constexpr void add_structural_component_bits(
      SchedulerMask& mask,
      mp::mp_list<ComponentsT...>);

  template <typename Component>
  static constexpr void add_structural_component_bit(SchedulerMask& mask);

  template <typename... Indices>
  static constexpr void add_view_write_bits(SchedulerMask& mask,
                                            mp::mp_list<Indices...>);

  template <typename SystemT>
  static void run_system_static(World& world);

  template <typename SystemT>
  static std::size_t system_work_static(const World& world);

  template <typename... Systems>
  static constexpr std::array<RuntimeSystemInfo, sizeof...(Systems)> make_system_infos(
      mp::mp_list<Systems...>);

  template <typename SystemT>
  void initialize_system_instance();

  template <typename SystemT,
            typename... ReadSingletons,
            typename... WriteSingletons>
  void initialize_system_instance(mp::mp_list<ReadSingletons...>,
                                  mp::mp_list<WriteSingletons...>);

  template <typename... SystemsT>
  void initialize_system_instances(mp::mp_list<SystemsT...>);

  void initialize_group_tree();
  void add_core_group(GroupPolicy policy);

  template <typename... Units>
  void initialize_registered_units(mp::mp_list<Units...>);

  template <typename Unit>
  void insert_initial_unit(GroupId parent, SchedulePosition position);

  template <typename... Units>
  void insert_initial_units(GroupId parent, mp::mp_list<Units...>);

  GroupId create_group_node(GroupPolicy policy, bool fixed);
  std::size_t create_entry(UnitKind kind, std::size_t id, SchedulePosition position);
  void attach_group_entry(std::size_t parentGroup, std::size_t childGroup, SchedulePosition position);
  void attach_entry(std::size_t parentGroup, std::size_t entryIndex);
  void detach_entry(std::size_t entryIndex);
  void mark_ancestors_dirty(std::size_t groupIndex);
  bool valid_group(GroupId group) const;
  bool is_group_ancestor_or_self(std::size_t ancestor, std::size_t child) const;
  bool insert_system_by_index(std::size_t systemIndex, GroupId parent, SchedulePosition position);
  bool remove_system_by_index(std::size_t systemIndex);
  bool move_system_by_index(std::size_t systemIndex, GroupId newParent);
  void destroy_group_contents(std::size_t groupIndex);

  void ensure_execution_plan();
  void rebuild_execution_plan();
  ExecutionPlan build_group_execution_plan(std::size_t groupIndex) const;
  ExecutionPlan build_entry_execution_plan(std::size_t entryIndex) const;
  ExecutionPlan build_entries_sequence_execution_plan(const std::vector<std::size_t>& entries) const;
  ExecutionPlan build_parallel_batch_execution_plan(const std::vector<std::size_t>& batch) const;
  static void append_execution_plan(ExecutionPlan& target, ExecutionPlan&& source);
  static void merge_parallel_execution_plan(ExecutionPlan& target, ExecutionPlan&& source);
  void execute_plan();
  void execute_step(const ExecutionStep& step);
  void execute_system_entries(const std::vector<std::size_t>& entries);
  void execute_system_entry(std::size_t entryIndex);
  void ensure_thread_pool();
  void rebuild_group(std::size_t groupIndex);
  SchedulerMask entry_mask(std::size_t entryIndex) const;
  std::size_t estimate_entries_work(const std::vector<std::size_t>& entries) const;
  std::size_t estimate_entry_work(std::size_t entryIndex) const;
  std::size_t estimate_group_work(std::size_t groupIndex) const;

  template <typename SystemT>
  void update_system();

  template <typename Component>
  void propagate_cached_component(Entity entity, const Component& value, void* skipView);

  template <typename Component, typename ViewT>
  static void sync_cached_component_to_view(ViewT& view,
                                            Entity entity,
                                            const Component& value,
                                            void* skipView);

  template <typename Component>
  void reevaluate_related_views(Entity entity);

  template <typename ViewT>
  void reevaluate_view(ViewT& view, Entity entity);

  void reevaluate_all_views(Entity entity);

  detail::EntityRegistry entities_;
  SourceTables sources_;
  ViewTables views_;
  SystemInstances systems_;
  std::array<RuntimeSystemInfo, SystemCount> systemInfos_ = make_system_infos(SystemList{});
  std::array<std::size_t, SystemCount> systemEntries_{};
  SchedulerConfig schedulerConfig_;
  detail::JThreadPool threadPool_;
  std::vector<GroupNode> groups_;
  std::vector<UnitEntry> entries_;
  ExecutionPlan executionPlan_;
  bool executionPlanDirty_ = true;
};

} // namespace snapecs

#include <SnapECS/detail/WorldComponentApi.hpp>
#include <SnapECS/detail/WorldScheduler.hpp>
#include <SnapECS/detail/WorldViewSync.hpp>
