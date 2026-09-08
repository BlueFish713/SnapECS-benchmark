#pragma once

#include <type_traits>

#include <SnapECS/Core/Types.hpp>

namespace snapecs {

// StoragePolicy 타입
// using StoragePolicy = 로 사용
struct CachedComponent {};  // System View Table에 복사되어 저장
struct IndexedComponent {}; // Entity ID로 저장
struct TagComponent {};     // Query 조건에만 사용
struct SingleComponent {};  // Singleton

// World에 등록된 컴포넌트 목록
template <typename... ComponentTs>
struct Components {
    using List = mp::mp_list<ComponentTs...>;
};

// 인스턴스화될 때 flase
template <typename>
inline constexpr bool dependent_false_v = false;

// ComponentStoragePolicy의 기본 템플릿
template <typename Component, typename = void>
struct ComponentStoragePolicy {
    static_assert(dependent_false_v<Component>, "Every component must define 'using StoragePolicy = ...'.");
};

// ComponentStoragePolicy의 부분 특수화
// using StoragePolicy = 를 작성한 경우
template <typename Component>
struct ComponentStoragePolicy<Component, std::void_t<typename Component::StoragePolicy>> {
    using Type = typename Component::StoragePolicy;
};

// alias template
template <typename Component>
using ComponentStoragePolicyT = typename ComponentStoragePolicy<Component>::Type;

// variable template
template <typename Component>
inline constexpr bool IsCachedComponentV =
    std::is_same_v<ComponentStoragePolicyT<Component>, CachedComponent>;

template <typename Component>
inline constexpr bool IsIndexedComponentV =
    std::is_same_v<ComponentStoragePolicyT<Component>, IndexedComponent>;

template <typename Component>
inline constexpr bool IsTagComponentV =
    std::is_same_v<ComponentStoragePolicyT<Component>, TagComponent>;

template <typename Component>
inline constexpr bool IsSingleComponentV =
    std::is_same_v<ComponentStoragePolicyT<Component>, SingleComponent>;

} // namespace snapecs
