#pragma once

#include <cstddef>

#include <boost/mp11.hpp>

namespace snapecs {

namespace mp = boost::mp11;

// Entity는 unsigned int ID와 같음
using Entity = unsigned int;

// System Group을 식별하는 핸들
struct GroupId {
    static constexpr std::size_t InvalidValue = static_cast<std::size_t>(-1);

    std::size_t value = InvalidValue;

    constexpr bool is_valid() const { return value != InvalidValue; }

    constexpr explicit operator bool() const { return is_valid(); }

    friend constexpr bool operator==(GroupId lhs, GroupId rhs) {
        return lhs.value == rhs.value;
    }

    friend constexpr bool operator!=(GroupId lhs, GroupId rhs) {
        return !(lhs == rhs);
    }
};

// System 스케줄러 설정
struct SchedulerConfig {
    // workerCount가 0이면 하드웨어 스레드 수를 기준으로 자동 결정함.
    std::size_t workerCount = 0; // worker 수

    // minParallelWork보다 batch 추정 작업량이 작으면 병렬 실행 대신 순차 실행
    std::size_t minParallelWork = 1024; // 최소 작업량
};

} // namespace snapecs
