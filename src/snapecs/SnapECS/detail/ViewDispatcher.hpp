#pragma once

#include <cstddef>
#include <tuple>

#include <SnapECS/detail/SystemViewTable.hpp>

namespace snapecs::detail {

// ViewSignatureList와 ComponentList를 SystemViewTable tuple로 변환
template <typename SignatureList, typename ComponentList>
struct ViewTableTuple;

template <typename... Signatures, typename ComponentList>
struct ViewTableTuple<mp::mp_list<Signatures...>, ComponentList> {
  // 고유 ViewSignature 하나당 SystemViewTable 하나
  using Type = std::tuple<SystemViewTable<Signatures, ComponentList>...>;
};

template <typename SignatureList, typename ComponentList>
using ViewTableTupleT = typename ViewTableTuple<SignatureList, ComponentList>::Type;

// tuple에서 컴파일 타임 index로 원소를 꺼냅니다.
// decltype(auto)는 std::get이 반환하는 참조/const 성질을 그대로 보존합니다.
template <std::size_t Index, typename Tuple>
decltype(auto) get_by_index(Tuple& tuple) {
  return std::get<Index>(tuple);
}

template <std::size_t Index, typename Tuple>
decltype(auto) get_by_index(const Tuple& tuple) {
  return std::get<Index>(tuple);
}

// World가 related view나 cached sync view만 갱신할 때 사용
template <typename ViewTuple>
struct ViewDispatcher {
  template <typename IndexList, typename Fn>
  static void for_each(ViewTuple& views, Fn&& fn) {
    for_each_impl(views, fn, IndexList{});
  }

private:
  template <typename Fn, typename... Indices>
  static void for_each_impl(ViewTuple& views, Fn& fn, mp::mp_list<Indices...>) {
    (fn(std::get<Indices::value>(views)), ...); //index의 value를 fn 함수에 전달
  }
};

} // namespace snapecs::detail
