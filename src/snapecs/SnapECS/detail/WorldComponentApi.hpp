#pragma once

#include <tuple>
#include <utility>

namespace snapecs {

// 엔티티 생성
template <typename... RegisteredComponents, typename... RegisteredUnits>
Entity World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::create_entity(bool active) {
  return entities_.create(active);
}

// 엔티티 활성화 여부
template <typename... RegisteredComponents, typename... RegisteredUnits>
bool World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::is_active(Entity entity) const {
  return entities_.is_active(entity);
}

// 엔티티 활성화
template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::activate_entity(Entity entity) {
  entities_.activate(entity);
  reevaluate_all_views(entity);
}

// 엔티티 비활성화
template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::deactivate_entity(Entity entity) {
  if (!entities_.deactivate(entity)) {
    return;
  }

  // 모든 view에 remove를 예약
  std::apply([&](auto&... views) { (views.enqueue_remove(entity), ...); }, views_);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component, typename... Args>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::add_component(Entity entity, Args&&... args) {
  static_assert(detail::ContainsV<ComponentList, Component>,
                "Component must be registered in World<Components<...>>.");
  static_assert(!IsSingleComponentV<Component>,
                "Single components are accessed through single_component<T>().");
  entities_.ensure_slot(entity);

  if constexpr (IsTagComponentV<Component>) {
    source_table<Component>().add(entity);
    reevaluate_related_views<Component>(entity);
  } else {
    auto& component = source_table<Component>().set(entity, std::forward<Args>(args)...);
    reevaluate_related_views<Component>(entity);

    if constexpr (IsCachedComponentV<Component>) {
      propagate_cached_component<Component>(entity, component, nullptr);
    }
  }
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::remove_component(Entity entity) {
  static_assert(detail::ContainsV<ComponentList, Component>,
                "Component must be registered in World<Components<...>>.");
  static_assert(!IsSingleComponentV<Component>,
                "Single components cannot be removed from a World.");

  source_table<Component>().remove(entity);
  reevaluate_related_views<Component>(entity);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component>
bool World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::has_component(Entity entity) const {
  static_assert(detail::ContainsV<ComponentList, Component>,
                "Component must be registered in World<Components<...>>.");
  static_assert(!IsSingleComponentV<Component>,
                "Single components always exist; use single_component<T>().");
  return source_table<Component>().contains(entity);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component>
Component& World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::get_component(Entity entity) {
  static_assert(detail::ContainsV<ComponentList, Component>,
                "Component must be registered in World<Components<...>>.");
  static_assert(!IsTagComponentV<Component>, "Tag components do not have data.");
  static_assert(!IsSingleComponentV<Component>,
                "Single components are accessed through single_component<T>().");
  return source_table<Component>().get(entity);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component>
const Component& World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::get_component(Entity entity) const {
  static_assert(detail::ContainsV<ComponentList, Component>,
                "Component must be registered in World<Components<...>>.");
  static_assert(!IsTagComponentV<Component>, "Tag components do not have data.");
  static_assert(!IsSingleComponentV<Component>,
                "Single components are accessed through single_component<T>().");
  return source_table<Component>().get(entity);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component, typename... Args>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::set_component(Entity entity, Args&&... args) {
  static_assert(detail::ContainsV<ComponentList, Component>,
                "Component must be registered in World<Components<...>>.");
  static_assert(!IsTagComponentV<Component>, "Tag components do not have data.");
  static_assert(!IsSingleComponentV<Component>,
                "Single components are modified through single_component<T>().");

  entities_.ensure_slot(entity);
  const bool hadComponent = has_component<Component>(entity);
  auto& component = source_table<Component>().set(entity, std::forward<Args>(args)...);

  // 없던 component가 생긴 경우에만 view membership이 바뀔 수 있습니다.
  if (!hadComponent) {
    reevaluate_related_views<Component>(entity);
  }

  if constexpr (IsCachedComponentV<Component>) {
    propagate_cached_component<Component>(entity, component, nullptr);
  }
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component, typename Fn>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::update_component(Entity entity, Fn&& fn) {
  static_assert(detail::ContainsV<ComponentList, Component>,
                "Component must be registered in World<Components<...>>.");
  static_assert(!IsSingleComponentV<Component>,
                "Single components are modified through single_component<T>().");

  if constexpr (!IsTagComponentV<Component>) {
    auto& component = source_table<Component>().get(entity);
    fn(component);

    if constexpr (IsCachedComponentV<Component>) {
      propagate_cached_component<Component>(entity, component, nullptr);
    }
  }
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component>
Component& World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::single_component() {
  static_assert(detail::ContainsV<ComponentList, Component>,
                "Component must be registered in World<Components<...>>.");
  static_assert(IsSingleComponentV<Component>,
                "single_component<T>() requires StoragePolicy = SingleComponent.");
  return source_table<Component>().get();
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component>
const Component& World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::single_component() const {
  static_assert(detail::ContainsV<ComponentList, Component>,
                "Component must be registered in World<Components<...>>.");
  static_assert(IsSingleComponentV<Component>,
                "single_component<T>() requires StoragePolicy = SingleComponent.");
  return source_table<Component>().get();
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT>
SystemT& World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::system() {
  static_assert(detail::ContainsV<SystemList, SystemT>,
                "System must be registered in World<..., Systems<...>>.");
  return *detail::get_by_index<SystemId<SystemT>>(systems_);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT>
const SystemT& World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::system() const {
  static_assert(detail::ContainsV<SystemList, SystemT>,
                "System must be registered in World<..., Systems<...>>.");
  return *detail::get_by_index<SystemId<SystemT>>(systems_);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT>
auto& World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::view_for() {
  return detail::get_by_index<ViewId<SystemT>>(views_);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT>
const auto& World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::view_for() const {
  return detail::get_by_index<ViewId<SystemT>>(views_);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component>
auto& World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::source_table() {
  static_assert(detail::ContainsV<ComponentList, Component>,
                "Component must be registered in World<Components<...>>.");
  return std::get<detail::ComponentSourceTable<Component>>(sources_);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component>
const auto& World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::source_table() const {
  static_assert(detail::ContainsV<ComponentList, Component>,
                "Component must be registered in World<Components<...>>.");
  return std::get<detail::ComponentSourceTable<Component>>(sources_);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::sync_cached_component(
    Entity entity,
    const Component& value,
    void* skipView) {
  static_assert(IsCachedComponentV<Component>,
                "Only cached components need System View Table synchronization.");

  source_table<Component>().set(entity, value);
  propagate_cached_component<Component>(entity, value, skipView);
}

} // namespace snapecs
