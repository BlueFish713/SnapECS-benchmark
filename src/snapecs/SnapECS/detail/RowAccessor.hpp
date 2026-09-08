#pragma once

#include <cstddef>

#include <SnapECS/Core.hpp>

namespace snapecs::detail {

// systemViewTable의 row 하나에서 component reference를 꺼냄
template <typename ViewT, typename WorldT>
class RowAccessor {
public:
  RowAccessor(ViewT &view, WorldT &world, Entity entity, bool bufferOdd,
              std::size_t row)
      : view_(view), world_(world), entity_(entity), bufferOdd_(bufferOdd),
        row_(row) {}

  Entity entity() const { return entity_; }
  
  //read는 const ref
  template <typename Component>
  const Component &read() const {
    static_assert(
        ViewT::template CanRead<Component> || ViewT::template CanWrite<Component>,
        "The component is not declared in this system's Read or Write list.");
    static_assert(!IsTagComponentV<Component>,
                  "Tag components do not have readable data.");

    if constexpr (IsCachedComponentV<Component>) {
      // CachedComponent는 cached buffer에서 가져오기
      return view_.template cached_at<Component>(bufferOdd_, row_);
    } else {
      // IndexedComponent는 원본 Sparse Set에서 가져오기
      return world_.template source_table<Component>().get(entity_);
    }
  }

  // read는 ref
  template <typename Component>
  Component &write() {
    static_assert(ViewT::template CanWrite<Component>,
                  "The component is not declared in this system's Write list.");
    static_assert(!IsTagComponentV<Component>,
                  "Tag components do not have writable data.");

    if constexpr (IsCachedComponentV<Component>) {
      // CachedComponent는 cached buffer에서 가져오기(동기화는 cached_at 내부에서 처리)
      return view_.template cached_at<Component>(bufferOdd_, row_);
    } else {
      // IndexedComponent는 원본 Sparse Set에서 가져오기
      return world_.template source_table<Component>().get(entity_);
    }
  }

private:
  ViewT &view_;
  WorldT &world_;
  Entity entity_;
  bool bufferOdd_;
  std::size_t row_;
};

} // namespace snapecs::detail
