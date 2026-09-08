#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace snapecs::detail {

// 병렬 스케줄러는 read/write bit mask로 충돌 여부를 빠르게 판단합니다.
// bit 0..ComponentCount-1은 컴포넌트 접근을 나타내고,
// 그 뒤 bit들은 각 SystemViewTable을 write 자원처럼 표시해 같은 view 병렬 실행을 막습니다.
template <std::size_t WordCount>
struct AccessMask {
  std::array<std::uint64_t, WordCount> read{};
  std::array<std::uint64_t, WordCount> write{};
};

// bit 번호를 64비트 word index와 word 내부 offset으로 나누어 세팅합니다.
template <std::size_t WordCount>
constexpr void set_mask_bit(std::array<std::uint64_t, WordCount>& words, std::size_t bit) {
  words[bit / 64] |= std::uint64_t{1} << (bit % 64);
}

// 두 bitset이 하나라도 같은 bit를 가지고 있는지 검사합니다.
template <std::size_t WordCount>
constexpr bool mask_intersects(const std::array<std::uint64_t, WordCount>& lhs,
                               const std::array<std::uint64_t, WordCount>& rhs) {
  for (std::size_t index = 0; index < WordCount; ++index) {
    if ((lhs[index] & rhs[index]) != 0) {
      return true;
    }
  }
  return false;
}

// 두 system/group access mask가 병렬 실행 시 충돌하는지 판단합니다.
// write-read, write-write가 겹치면 충돌입니다. read-read는 충돌이 아닙니다.
template <std::size_t WordCount>
constexpr bool masks_conflict(const AccessMask<WordCount>& lhs,
                              const AccessMask<WordCount>& rhs) {
  for (std::size_t index = 0; index < WordCount; ++index) {
    const auto lhsWrites = lhs.write[index];
    const auto rhsWrites = rhs.write[index];
    if ((lhsWrites & (rhs.read[index] | rhsWrites)) != 0) {
      return true;
    }
    if ((rhsWrites & lhs.read[index]) != 0) {
      return true;
    }
  }
  return false;
}

// group mask를 만들 때 child entry mask들을 누적합니다.
template <std::size_t WordCount>
constexpr void merge_mask(AccessMask<WordCount>& target, const AccessMask<WordCount>& source) {
  for (std::size_t index = 0; index < WordCount; ++index) {
    target.read[index] |= source.read[index];
    target.write[index] |= source.write[index];
  }
}

// 현재 코드에서는 보조 검사로 둘 수 있는 mask empty 판별입니다.
template <std::size_t WordCount>
constexpr bool mask_is_empty(const AccessMask<WordCount>& mask) {
  for (std::size_t index = 0; index < WordCount; ++index) {
    if (mask.read[index] != 0 || mask.write[index] != 0) {
      return false;
    }
  }
  return true;
}

} // namespace snapecs::detail
