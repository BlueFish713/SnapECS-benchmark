#pragma once

#include <SnapECS/Core/Types.hpp>

namespace snapecs {

// SystemGroup의 실행 정책
enum class GroupPolicy {
    Parallel,   // 병렬 처리
    NonParallel // 순차 처리
};

// system/group의 위치
enum class SchedulePosition {
    Auto,           // 병렬 batch에 배치될 수 있음
    BeforeParallel, // 병렬 batch 앞에 배치
    AfterParallel   // 병렬 batch 뒤에 배치
};

// SystemGroup
// NameTag는 그룹 이름 역할을 하는 빈 타입
template <typename NameTag, GroupPolicy Policy = GroupPolicy::Parallel, typename... Units>
struct SystemGroup {
    using Name = NameTag;
    using UnitList = mp::mp_list<Units...>;

    static constexpr GroupPolicy PolicyValue = Policy;
};

// Scheduled
// Unit에 SchedulePosition를 붙임
template <typename Unit, SchedulePosition Position>
struct Scheduled {
    using UnitType = Unit;

    static constexpr SchedulePosition PositionValue = Position;
};

} // namespace snapecs
