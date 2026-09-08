#pragma once

#include <SnapECS/Core/Query.hpp>

namespace snapecs::detail {

// 기본 템플릿
template <typename T>
struct IncludeToList {
    using Type = mp::mp_list<>;
};

// Include<Components...> 모양이면 Include 내부 List를 추출
template <typename... Components>
struct IncludeToList<Include<Components...>> {
    using Type = typename Include<Components...>::List;
};

// 기본 템플릿
template <typename T>
struct ExcludeToList {
    using Type = mp::mp_list<>;
};

// Exclude<Components...> 모양이면 Exclude 내부 List를 추출
template <typename... Components>
struct ExcludeToList<Exclude<Components...>> {
    using Type = typename Exclude<Components...>::List;
};

// Query<Terms...>에서 Include<...> term 추출
template <typename QueryT>
struct QueryIncludes;

template <typename... Terms>
struct QueryIncludes<Query<Terms...>> {
    using Type =
        mp::mp_unique<mp::mp_append<typename IncludeToList<Terms>::Type...>>;
};

// Query<Terms...>에서 Exclude<...> term 추출
template <typename QueryT>
struct QueryExcludes;

template <typename... Terms>
struct QueryExcludes<Query<Terms...>> {
    using Type =
        mp::mp_unique<mp::mp_append<typename ExcludeToList<Terms>::Type...>>;
};

// alias template
template <typename QueryT>
using QueryIncludesT = typename QueryIncludes<QueryT>::Type;

// alias template
template <typename QueryT>
using QueryExcludesT = typename QueryExcludes<QueryT>::Type;

} // namespace snapecs::detail
