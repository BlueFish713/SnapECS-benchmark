#pragma once

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <SnapECS/Core.hpp>

namespace snapecs::detail {

// ComponentSourceTable 원본 저장소
template <typename Component,
          bool IsTag = IsTagComponentV<Component>,
          bool IsSingle = IsSingleComponentV<Component>>
class ComponentSourceTable;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CachedComponent와 IndexedComponent 용 source table
template <typename Component>
class ComponentSourceTable<Component, false, false> {
  public:
    // Npos는 컴포넌트가 없는 Entity를 나타냄.
    static constexpr std::size_t Npos =
        std::numeric_limits<std::size_t>::max();

    bool contains(Entity entity) const {
        const auto index = sparse_index(entity);

        // 범위 확인
        return index != Npos && index < denseEntities_.size() &&
               denseEntities_[index] == entity;
    }

    template <typename... Args>
    Component &set(Entity entity, Args &&...args) {
        // Entity ID를 sparse_ 배열 인덱스로 쓸 수 있도록 공간을 확보합니다.
        ensure_sparse(entity);

        if (contains(entity)) {
            // 이미 컴포넌트가 있으면 새 row를 만들지 않고 기존 데이터를 덮어씁니다.
            // std::forward는 전달받은 인자의 lvalue/rvalue 성질을 유지합니다.
            denseData_[sparse_[entity]] = Component{std::forward<Args>(args)...};
            return denseData_[sparse_[entity]];
        }

        // 새 컴포넌트면 dense 배열 맨 뒤에 추가합니다.
        const auto index = denseEntities_.size();
        sparse_[entity] = index;
        denseEntities_.push_back(entity);
        denseData_.push_back(Component{std::forward<Args>(args)...});
        return denseData_.back();
    }

    bool remove(Entity entity) {
        if (!contains(entity)) {
            return false;
        }

        const auto index = sparse_[entity];
        const auto lastIndex = denseEntities_.size() - 1;
        const auto lastEntity = denseEntities_[lastIndex];

        if (index != lastIndex) {
            // swap-remove: 삭제할 row에 마지막 row를 옮겨 빈틈을 없앱니다.
            // 이 방식은 빠르지만 dense 배열 순서는 보존하지 않습니다.
            denseEntities_[index] = lastEntity;
            denseData_[index] = std::move(denseData_[lastIndex]);
            sparse_[lastEntity] = index;
        }

        denseEntities_.pop_back();
        denseData_.pop_back();
        sparse_[entity] = Npos;
        return true;
    }

    Component &get(Entity entity) {
        if (!contains(entity)) {
            throw std::out_of_range(
                "ComponentSourceTable::get: component is missing.");
        }
        return denseData_[sparse_[entity]];
    }

    const Component &get(Entity entity) const {
        if (!contains(entity)) {
            throw std::out_of_range(
                "ComponentSourceTable::get: component is missing.");
        }
        return denseData_[sparse_[entity]];
    }

    std::size_t size() const { return denseEntities_.size(); }

  private:
    void ensure_sparse(Entity entity) {
        const auto required = static_cast<std::size_t>(entity) + 1;
        if (sparse_.size() < required) {
            sparse_.resize(required, Npos);
        }
    }

    std::size_t sparse_index(Entity entity) const {
        const auto index = static_cast<std::size_t>(entity);
        return index < sparse_.size() ? sparse_[index] : Npos;
    }

    std::vector<std::size_t> sparse_;
    std::vector<Entity> denseEntities_;
    std::vector<Component> denseData_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tag 컴포넌트 용 source table
template <typename Component>
class ComponentSourceTable<Component, true, false> {
  public:
    static constexpr std::size_t Npos = std::numeric_limits<std::size_t>::max();

    bool contains(Entity entity) const {
        const auto index = sparse_index(entity);
        return index != Npos && index < denseEntities_.size() &&
               denseEntities_[index] == entity;
    }

    bool add(Entity entity) {
        ensure_sparse(entity);
        if (contains(entity)) {
            return false;
        }

        sparse_[entity] = denseEntities_.size();
        denseEntities_.push_back(entity);
        return true;
    }

    bool remove(Entity entity) {
        if (!contains(entity)) {
            return false;
        }

        const auto index = sparse_[entity];
        const auto lastIndex = denseEntities_.size() - 1;
        const auto lastEntity = denseEntities_[lastIndex];

        if (index != lastIndex) {
            // tag에는 데이터가 없으므로 entity ID와 sparse index만 옮기면 됩니다.
            denseEntities_[index] = lastEntity;
            sparse_[lastEntity] = index;
        }

        denseEntities_.pop_back();
        sparse_[entity] = Npos;
        return true;
    }

    std::size_t size() const { return denseEntities_.size(); }

  private:
    void ensure_sparse(Entity entity) {
        const auto required = static_cast<std::size_t>(entity) + 1;
        if (sparse_.size() < required) {
            sparse_.resize(required, Npos);
        }
    }

    std::size_t sparse_index(Entity entity) const {
        const auto index = static_cast<std::size_t>(entity);
        return index < sparse_.size() ? sparse_[index] : Npos;
    }

    std::vector<std::size_t> sparse_;
    std::vector<Entity> denseEntities_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Single 컴포넌트 용 source table

template <typename Component>
class ComponentSourceTable<Component, false, true> {
  public:
    // 기본 생성 가능 여부 확인
    static_assert(std::is_default_constructible_v<Component>,
                  "Single components must be default constructible.");

    // noexcept => 예외 발생 X
    Component& get() noexcept { return component_; }
    const Component& get() const noexcept { return component_; }

    static constexpr std::size_t size() noexcept { return 1; }

  private:
    Component component_{};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 전체 source table을 tuple로 변환

template <typename ComponentList>
struct SourceTableTuple;

template <typename... Components>
struct SourceTableTuple<mp::mp_list<Components...>> {
    using Type = std::tuple<ComponentSourceTable<Components>...>;
};

template <typename ComponentList>
using SourceTableTupleT = typename SourceTableTuple<ComponentList>::Type;

} // namespace snapecs::detail
