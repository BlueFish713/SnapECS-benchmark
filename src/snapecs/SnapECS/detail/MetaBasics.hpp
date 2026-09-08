#pragma once

#include <type_traits>

#include <SnapECS/Core/Types.hpp>

namespace snapecs::detail {

// variable template
// 타입 리스트 안에 특정 타입이 있는지 검사
template <typename List, typename T>
inline constexpr bool ContainsV = mp::mp_contains<List, T>::value;

// 타입 리스트 안에 중복 타입이 있는지 검사
// 기본 템플릿
template <typename List>
struct TypeListIsUnique;

// 특수 템플릿
template <typename... Ts>
struct TypeListIsUnique<mp::mp_list<Ts...>>
    : std::bool_constant<
          sizeof...(Ts) == mp::mp_size<mp::mp_unique<mp::mp_list<Ts...>>>::value> {};

} // namespace snapecs::detail
