#pragma once

#include <SnapECS/Core/Component.hpp>

namespace snapecs::detail {

// Components<...> 안의 항목 하나를 평탄화된 컴포넌트 타입 목록으로 변환합니다.
// 일반 타입은 실제 컴포넌트이므로 단일 항목 목록이 됩니다.
template <typename Component>
struct ComponentTypeList {
    using Type = mp::mp_list<Component>;
};

// Components<...>는 등록 그룹이므로 내부 항목을 재귀적으로 펼칩니다.
template <typename... ComponentsT>
struct ComponentTypeList<Components<ComponentsT...>> {
    using Type = mp::mp_append<typename ComponentTypeList<ComponentsT>::Type...>;
};

template <typename Component>
using ComponentTypeListT = typename ComponentTypeList<Component>::Type;

// World에 전달된 최상위 컴포넌트 목록을 실제 컴포넌트 타입 목록으로 평탄화합니다.
template <typename ComponentList>
struct FlattenComponents;

template <typename... ComponentsT>
struct FlattenComponents<mp::mp_list<ComponentsT...>> {
    using Type = mp::mp_append<ComponentTypeListT<ComponentsT>...>;
};

template <typename ComponentList>
using FlattenComponentsT = typename FlattenComponents<ComponentList>::Type;

} // namespace snapecs::detail
