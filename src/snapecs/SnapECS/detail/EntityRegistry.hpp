#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <SnapECS/Core.hpp>

namespace snapecs::detail {

// EntityRegistry는 Entity ID 발급과 active 상태만 관리합니다.
// 컴포넌트 데이터나 view membership은 여기서 관리하지 않습니다.
// 시스템은 이 registry를 전체 스캔하지 않고, SystemViewTable이 관리하는 entity 목록만 순회합니다.
class EntityRegistry {
public:
  Entity create(bool active = true) {
    // nextEntity_++는 현재 값을 entity ID로 사용한 뒤 다음 ID를 위해 1 증가시킵니다.
    const Entity entity = nextEntity_++;

    // active_ 배열의 index가 Entity ID와 대응됩니다.
    active_.push_back(active ? std::uint8_t{1} : std::uint8_t{0});
    return entity;
  }

  bool is_active(Entity entity) const {
    const auto index = static_cast<std::size_t>(entity);

    // 범위 밖 Entity는 존재하지 않거나 슬롯이 없으므로 inactive로 취급합니다.
    return index < active_.size() && active_[index] != 0;
  }

  void ensure_slot(Entity entity) {
    const auto required = static_cast<std::size_t>(entity) + 1;
    if (active_.size() < required) {
      // 임의의 큰 Entity ID에 component를 붙이거나 activate할 수 있도록 슬롯을 만듭니다.
      // 새 슬롯은 inactive입니다.
      active_.resize(required, 0);

      // 다음 create()가 이미 확보된 ID 범위 뒤에서 시작하도록 맞춥니다.
      nextEntity_ = static_cast<Entity>(active_.size());
    }
  }

  void activate(Entity entity) {
    ensure_slot(entity);
    active_[entity] = 1;
  }

  bool deactivate(Entity entity) {
    const auto index = static_cast<std::size_t>(entity);
    if (index >= active_.size()) {
      return false;
    }

    active_[entity] = 0;
    return true;
  }

private:
  Entity nextEntity_ = 0;
  std::vector<std::uint8_t> active_;
};

} // namespace snapecs::detail
