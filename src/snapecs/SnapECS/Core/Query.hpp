#pragma once

#include <cstddef>
#include <type_traits>

#include <SnapECS/Core/Component.hpp>

namespace snapecs {

// Include
template <typename... ComponentTs> struct Include {
    static_assert((!IsSingleComponentV<ComponentTs> && ...), "Single components cannot be used in Query.");

    // mp_unique로 중복 타입 제거
    using List = mp::mp_unique<mp::mp_list<ComponentTs...>>;
};

// Exclude
template <typename... ComponentTs> struct Exclude {
    static_assert((!IsSingleComponentV<ComponentTs> && ...), "Single components cannot be used in Query.");

    // mp_unique로 중복 타입 제거
    using List = mp::mp_unique<mp::mp_list<ComponentTs...>>;
};

namespace detail {

// IsIncludeTerm과 IsExcludeTerm은 Include<...> 또는 Exclude<...>인지 판별하는 타입

// 기본 템플릿
template <typename T> struct IsIncludeTerm : std::false_type {};

// 타입이 Include<...> 모양일 때만 true_type이 되도록 부분 특수화
template <typename... ComponentTs> struct IsIncludeTerm<Include<ComponentTs...>> : std::true_type {};

// Exclude도 같은 방식으로 판별합니다.
template <typename T> struct IsExcludeTerm : std::false_type {};

template <typename... ComponentTs> struct IsExcludeTerm<Exclude<ComponentTs...>> : std::true_type {};

} // namespace detail

// Query
template <typename... Terms> struct Query {
  private:
    // ... + (계산식)은 Terms 내부의 타입을 펼쳐서 계산 (fold expression)
    // Include term 개수
    static constexpr std::size_t IncludeTermCount = (std::size_t{0} + ... + (detail::IsIncludeTerm<Terms>::value ? std::size_t{1} : std::size_t{0}));

    // Exclude term 개수
    static constexpr std::size_t ExcludeTermCount = (std::size_t{0} + ... + (detail::IsExcludeTerm<Terms>::value ? std::size_t{1} : std::size_t{0}));

  public:
    // ... Terms에 Include/Exclude가 아닌 타입을 넣으면 에러
    static_assert(((detail::IsIncludeTerm<Terms>::value || detail::IsExcludeTerm<Terms>::value) && ...), "Query terms must be Include<...> or Exclude<...>.");

    // Include<...>와 Exclude<...> 는 각각 하나만 존재
    static_assert(IncludeTermCount <= 1, "Query can contain at most one Include<...> term.");
    static_assert(ExcludeTermCount <= 1, "Query can contain at most one Exclude<...> term.");
};

} // namespace snapecs
