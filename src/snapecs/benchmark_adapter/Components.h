#ifndef ECS_BENCHMARKS_SNAPECS_COMPONENTS_H_
#define ECS_BENCHMARKS_SNAPECS_COMPONENTS_H_

#include "base/components/DataComponent.h"
#include "base/components/EmptyComponent.h"
#include "base/components/HeroMonsterComponents.h"
#include "base/components/PositionComponent.h"
#include "base/components/SpriteComponent.h"
#include "base/components/VelocityComponent.h"

#include <SnapECS/Core/Component.hpp>

namespace ecs::benchmarks::snapecs::components {

template <class Base, class StoragePolicyT>
struct StoredComponent : Base {
  using StoragePolicy = StoragePolicyT;
};

struct EmptyComponent : base::components::EmptyComponent {
  using StoragePolicy = ::snapecs::TagComponent;
};

template <class StoragePolicyT>
struct ComponentSet {
  using PositionComponent = StoredComponent<base::components::PositionComponent, StoragePolicyT>;
  using VelocityComponent = StoredComponent<base::components::VelocityComponent, StoragePolicyT>;
  using DataComponent = StoredComponent<base::components::DataComponent, StoragePolicyT>;
  using PlayerComponent = StoredComponent<base::components::PlayerComponent, StoragePolicyT>;
  using HealthComponent = StoredComponent<base::components::HealthComponent, StoragePolicyT>;
  using DamageComponent = StoredComponent<base::components::DamageComponent, StoragePolicyT>;
  using SpriteComponent = StoredComponent<base::components::SpriteComponent, StoragePolicyT>;
  using EmptyComponent = components::EmptyComponent;
};

using IndexedComponentSet = ComponentSet<::snapecs::IndexedComponent>;
using CachedComponentSet = ComponentSet<::snapecs::CachedComponent>;

// Preserve the original adapter API as indexed aliases.
using PositionComponent = IndexedComponentSet::PositionComponent;
using VelocityComponent = IndexedComponentSet::VelocityComponent;
using DataComponent = IndexedComponentSet::DataComponent;
using PlayerComponent = IndexedComponentSet::PlayerComponent;
using HealthComponent = IndexedComponentSet::HealthComponent;
using DamageComponent = IndexedComponentSet::DamageComponent;
using SpriteComponent = IndexedComponentSet::SpriteComponent;

static_assert(::snapecs::IsIndexedComponentV<IndexedComponentSet::PositionComponent>);
static_assert(::snapecs::IsIndexedComponentV<IndexedComponentSet::VelocityComponent>);
static_assert(::snapecs::IsIndexedComponentV<IndexedComponentSet::DataComponent>);
static_assert(::snapecs::IsCachedComponentV<CachedComponentSet::PositionComponent>);
static_assert(::snapecs::IsCachedComponentV<CachedComponentSet::VelocityComponent>);
static_assert(::snapecs::IsCachedComponentV<CachedComponentSet::DataComponent>);
static_assert(::snapecs::IsTagComponentV<EmptyComponent>);

} // namespace ecs::benchmarks::snapecs::components

#endif // ECS_BENCHMARKS_SNAPECS_COMPONENTS_H_
