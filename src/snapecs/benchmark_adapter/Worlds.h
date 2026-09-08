#ifndef ECS_BENCHMARKS_SNAPECS_WORLDS_H_
#define ECS_BENCHMARKS_SNAPECS_WORLDS_H_

#include "benchmark_adapter/Components.h"
#include "benchmark_adapter/Systems.h"

#include <SnapECS/SnapECS.hpp>

namespace ecs::benchmarks::snapecs {

struct SequentialSystemsName {};

template <class StoragePolicyT>
struct BenchmarkConfig {
  using ComponentTypes = components::ComponentSet<StoragePolicyT>;
  using SystemTypes = systems::SystemSet<ComponentTypes>;

  using BenchmarkComponents =
      ::snapecs::Components<typename ComponentTypes::PositionComponent, typename ComponentTypes::VelocityComponent,
                            typename ComponentTypes::DataComponent, typename ComponentTypes::EmptyComponent,
                            typename ComponentTypes::PlayerComponent, typename ComponentTypes::HealthComponent,
                            typename ComponentTypes::DamageComponent, typename ComponentTypes::SpriteComponent>;

  using SequentialSystems =
      ::snapecs::SystemGroup<SequentialSystemsName, ::snapecs::GroupPolicy::NonParallel,
                             typename SystemTypes::MovementSystem, typename SystemTypes::DataSystem,
                             typename SystemTypes::MoreComplexSystem, typename SystemTypes::HealthSystem,
                             typename SystemTypes::DamageSystem, typename SystemTypes::SpriteSystem,
                             typename SystemTypes::RenderSystem>;

  using EntityWorld = ::snapecs::World<BenchmarkComponents, ::snapecs::Systems<>>;
  using ApplicationWorld = ::snapecs::World<BenchmarkComponents, ::snapecs::Systems<SequentialSystems>>;
};

using IndexedBenchmarkConfig = BenchmarkConfig<::snapecs::IndexedComponent>;
using CachedBenchmarkConfig = BenchmarkConfig<::snapecs::CachedComponent>;

#if defined(SNAPECS_BENCHMARK_CACHED)
using SelectedBenchmarkConfig = CachedBenchmarkConfig;
#else
using SelectedBenchmarkConfig = IndexedBenchmarkConfig;
#endif

// Preserve the original adapter API as indexed aliases.
using BenchmarkComponents = IndexedBenchmarkConfig::BenchmarkComponents;
using SequentialSystems = IndexedBenchmarkConfig::SequentialSystems;
using EntityWorld = IndexedBenchmarkConfig::EntityWorld;
using ApplicationWorld = IndexedBenchmarkConfig::ApplicationWorld;

} // namespace ecs::benchmarks::snapecs

#endif // ECS_BENCHMARKS_SNAPECS_WORLDS_H_
