#pragma once

#include <type_traits>

#include <SnapECS/Core/Schedule.hpp>

namespace snapecs::detail {

// 기본 템플릿
template <typename T>
struct IsSystemGroup : std::false_type {};

// 타입이 SystemGroup<NameTag, Policy, Units...> 모양일 때만 true_type이 되도록 부분 특수화
template <typename NameTag, GroupPolicy Policy, typename... Units>
struct IsSystemGroup<SystemGroup<NameTag, Policy, Units...>> : std::true_type {};

// variable template
template <typename T>
inline constexpr bool IsSystemGroupV = IsSystemGroup<T>::value;

// 기본 템플릿
template <typename T>
struct IsScheduled : std::false_type {};

// 타입이 Scheduled<Unit, Position> 모양일 때만 true_type이 되도록 부분 특수화
template <typename Unit, SchedulePosition Position>
struct IsScheduled<Scheduled<Unit, Position>> : std::true_type {};

// variable template
template <typename T>
inline constexpr bool IsScheduledV = IsScheduled<T>::value;

// UnitSystemList는 Systems<...> 안의 unit 하나를 실제 system 타입 리스트로 바꿉니다.
// 단일 system인 경우 => 빈 리스트
template <typename Unit>
struct UnitSystemList {
    using Type = mp::mp_list<Unit>;
};

// Scheduled인 경우 => Position 제거 후 Unit만 추출
template <typename Unit, SchedulePosition Position>
struct UnitSystemList<Scheduled<Unit, Position>> {
    using Type = typename UnitSystemList<Unit>::Type;
};

// SystemGroup인 경우 => 내부 Unit을 다시 UnitSystemList로 변환 후 append
template <typename NameTag, GroupPolicy Policy, typename... Units>
struct UnitSystemList<SystemGroup<NameTag, Policy, Units...>> {
    using Type = mp::mp_append<typename UnitSystemList<Units>::Type...>;
};

// alias template
template <typename Unit>
using UnitSystemListT = typename UnitSystemList<Unit>::Type;

// Systems<...>에 들어온 최상위 unit 리스트를 실제 system 타입 리스트로 flatten합니다.
template <typename UnitList>
struct FlattenUnits;

template <typename... Units>
struct FlattenUnits<mp::mp_list<Units...>> {
    using Type = mp::mp_append<UnitSystemListT<Units>...>;
};

template <typename UnitList>
using FlattenUnitsT = typename FlattenUnits<UnitList>::Type;

} // namespace snapecs::detail
