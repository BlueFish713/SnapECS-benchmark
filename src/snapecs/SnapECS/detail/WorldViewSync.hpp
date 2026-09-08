#pragma once

#include <tuple>

namespace snapecs {

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::update_system() {
  auto& view = view_for<SystemT>();
  view.template for_each<SystemT>(*this, system<SystemT>());
}

// cached 컴포넌트를 전파
// add, set, update에서 call
template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::propagate_cached_component(
    Entity entity,
    const Component& value,
    void* skipView) {
  using Indices = CachedSyncViewIndices<Component>; // 동기화할 View Index 찾기

  detail::ViewDispatcher<ViewTables>::template for_each<Indices>( // View 순회하며 sync
      views_, [&](auto& view) {
        sync_cached_component_to_view<Component>(view, entity, value, skipView);
      });
}

// cached 컴포넌트를 view에 동기화
// propagate_cached_component에서 호출
template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component, typename ViewT>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::sync_cached_component_to_view(
    ViewT& view,
    Entity entity,
    const Component& value,
    void* skipView) {
  if constexpr (ViewT::template StoresCached<Component>) { // 해당 view에 cached component가 저장될 수 있음
    if (static_cast<void*>(&view) != skipView) {
      view.template update_cached_copy_if_present<Component>(entity, value);
    }
  }
}

// 연관된 view를 검사
template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::reevaluate_related_views(Entity entity) {
  using Indices = RelatedViewIndices<Component>;

  detail::ViewDispatcher<ViewTables>::template for_each<Indices>(
      views_, [&](auto& view) { reevaluate_view(view, entity); });
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename ViewT>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::reevaluate_view(
    ViewT& view,
    Entity entity) {
  if (view.matches(*this, entity)) {
    view.enqueue_add(entity);
  } else {
    view.enqueue_remove(entity);
  }
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::reevaluate_all_views(Entity entity) {
  std::apply(
      [&](auto&... views) {
        ((views.matches(*this, entity) ? views.enqueue_add(entity) : views.enqueue_remove(entity)),
         ...);
      },
      views_);
}

} // namespace snapecs
