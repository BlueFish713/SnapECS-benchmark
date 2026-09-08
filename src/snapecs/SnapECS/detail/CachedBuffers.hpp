#pragma once

#include <cstddef>
#include <tuple>
#include <vector>

#include <SnapECS/Core.hpp>

namespace snapecs::detail {

// vector가 count개 row를 담을 수 있게 크기를 늘립니다.
// 이미 충분히 크면 아무 작업도 하지 않습니다.
template <typename Component>
void EnsureBufferSize(std::vector<Component>& buffer, std::size_t count) {
  if (buffer.size() < count) {
    buffer.resize(count);
  }
}

template <typename ComponentList>
class CachedBuffers;

// CachedBuffers는 SystemViewTable이 저장해야 하는 CachedComponent 복사본들을 관리합니다.
// ComponentList가 mp_list<Position, Health>라면 내부 tuple은
// tuple<vector<Position>, vector<Health>>가 됩니다.
//
// odd_/even_ 두 벌을 두는 이유:
// pending add/remove를 병합할 때 현재 순회 snapshot은 source buffer로 읽고,
// 새 view 상태는 target buffer에 씁니다. 다음 실행부터 target이 active가 됩니다.
template <typename... Components>
class CachedBuffers<mp::mp_list<Components...>> {
public:
  template <typename Component>
  Component& at(bool oddBuffer, std::size_t row) {
    // std::get<std::vector<Component>>는 tuple 안에서 해당 타입의 vector를 꺼냅니다.
    return std::get<std::vector<Component>>(tuple(oddBuffer))[row];
  }

  template <typename Component>
  const Component& at(bool oddBuffer, std::size_t row) const {
    return std::get<std::vector<Component>>(tuple(oddBuffer))[row];
  }

  void ensure_size(bool oddBuffer, std::size_t count) {
    // comma fold expression으로 모든 cached component vector를 같은 row 수까지 늘립니다.
    (EnsureBufferSize(std::get<std::vector<Components>>(tuple(oddBuffer)), count), ...);
  }

  template <typename WorldT>
  void copy_from_source(bool targetOdd, std::size_t targetRow, WorldT& world, Entity entity) {
    // 새로 view에 들어오는 entity는 source table 원본 값을 target cached buffer로 복사합니다.
    // world.template source_table<Components>()에서 template은 dependent object의 템플릿 멤버 호출 표시입니다.
    ((at<Components>(targetOdd, targetRow) =
          world.template source_table<Components>().get(entity)),
     ...);
  }

  // target buffer에서 source buffer로 값 이동 시 사용
  void copy_between(bool sourceOdd, std::size_t sourceRow, bool targetOdd, std::size_t targetRow) {
    ((at<Components>(targetOdd, targetRow) = at<Components>(sourceOdd, sourceRow)), ...);
  }

private:
  auto& tuple(bool oddBuffer) {
    return oddBuffer ? odd_ : even_;
  }

  const auto& tuple(bool oddBuffer) const {
    return oddBuffer ? odd_ : even_;
  }

  std::tuple<std::vector<Components>...> odd_;
  std::tuple<std::vector<Components>...> even_;
};

} // namespace snapecs::detail
