#ifndef ECS_BENCHMARKS_SNAPECS_EXTENDED_BENCHMARK_SUITE_H_
#define ECS_BENCHMARKS_SNAPECS_EXTENDED_BENCHMARK_SUITE_H_

#include "ExtendedECSBenchmark.h"
#include "benchmark_adapter/Application.h"
#include "benchmark_adapter/EntityFactory.h"
#include "benchmark_adapter/HeroMonsterEntityFactory.h"
#include "benchmark_adapter/Worlds.h"

#include <benchmark/benchmark.h>
#include <cstddef>
#include <utility>
#include <vector>

namespace ecs::benchmarks::snapecs {

namespace iteration {

template <class ComponentsT>
class OneComponentSystem final
    : public ::snapecs::System<::snapecs::Query<>, ::snapecs::Read<typename ComponentsT::PositionComponent>,
                               ::snapecs::Write<>> {
public:
  using PositionComponent = typename ComponentsT::PositionComponent;

  void BeginUpdate() { visited_ = 0; }

  void Update(::snapecs::Entity entity, const PositionComponent& position) override {
    benchmark::DoNotOptimize(entity);
    benchmark::DoNotOptimize(position);
    ++visited_;
  }

  [[nodiscard]] std::size_t visited() const { return visited_; }

private:
  std::size_t visited_{0};
};

template <class ComponentsT>
class TwoComponentSystem final
    : public ::snapecs::System<::snapecs::Query<>,
                               ::snapecs::Read<typename ComponentsT::PositionComponent,
                                               typename ComponentsT::VelocityComponent>,
                               ::snapecs::Write<>> {
public:
  using PositionComponent = typename ComponentsT::PositionComponent;
  using VelocityComponent = typename ComponentsT::VelocityComponent;

  void BeginUpdate() { visited_ = 0; }

  void Update(::snapecs::Entity entity, const PositionComponent& position,
              const VelocityComponent& velocity) override {
    benchmark::DoNotOptimize(entity);
    benchmark::DoNotOptimize(position);
    benchmark::DoNotOptimize(velocity);
    ++visited_;
  }

  [[nodiscard]] std::size_t visited() const { return visited_; }

private:
  std::size_t visited_{0};
};

template <class ComponentsT>
class ThreeComponentSystem final
    : public ::snapecs::System<
          ::snapecs::Query<>,
          ::snapecs::Read<typename ComponentsT::PositionComponent, typename ComponentsT::VelocityComponent,
                          typename ComponentsT::DataComponent>,
          ::snapecs::Write<>> {
public:
  using PositionComponent = typename ComponentsT::PositionComponent;
  using VelocityComponent = typename ComponentsT::VelocityComponent;
  using DataComponent = typename ComponentsT::DataComponent;

  void BeginUpdate() { visited_ = 0; }

  void Update(::snapecs::Entity entity, const PositionComponent& position, const VelocityComponent& velocity,
              const DataComponent& data) override {
    benchmark::DoNotOptimize(entity);
    benchmark::DoNotOptimize(position);
    benchmark::DoNotOptimize(velocity);
    benchmark::DoNotOptimize(data);
    ++visited_;
  }

  [[nodiscard]] std::size_t visited() const { return visited_; }

private:
  std::size_t visited_{0};
};

} // namespace iteration

using ActiveBenchmarkConfig = SelectedBenchmarkConfig;
using ActiveApplicationWorld = ActiveBenchmarkConfig::ApplicationWorld;
using ActiveComponentTypes = ActiveBenchmarkConfig::ComponentTypes;
using ActiveSystemTypes = ActiveBenchmarkConfig::SystemTypes;

using SnapECSExtendedApplication = Application<ActiveApplicationWorld, ActiveSystemTypes>;
using ExtendedEntityFactory = entities::EntityFactory<ActiveApplicationWorld, ActiveComponentTypes>;
using ExtendedHeroMonsterEntityFactory =
    entities::HeroMonsterEntityFactory<ActiveApplicationWorld, ActiveComponentTypes>;

#if defined(SNAPECS_BENCHMARK_CACHED)
#define ECS_BENCHMARKS_SNAPECS_NAME "snapecs-cached"
#else
#define ECS_BENCHMARKS_SNAPECS_NAME "snapecs-indexed"
#endif

class SnapECSExtendedBenchmarkSuite final
    : public base::ExtendedECSBenchmark<ECS_BENCHMARKS_SNAPECS_NAME, SnapECSExtendedApplication, ExtendedEntityFactory,
                                        ExtendedHeroMonsterEntityFactory> {
public:
  SnapECSExtendedBenchmarkSuite() = default;

  explicit SnapECSExtendedBenchmarkSuite(base::ESCBenchmarkOptions options)
      : ExtendedECSBenchmark(std::move(options)) {}

  void BM_IterateSingleComponent(benchmark::State& state) {
    run_iteration<iteration::OneComponentSystem<ActiveComponentTypes>>(state, EntityShape::Single);
  }

  void BM_IterateTwoComponents(benchmark::State& state) {
    run_iteration<iteration::TwoComponentSystem<ActiveComponentTypes>>(state, EntityShape::Minimal);
  }

  void BM_IterateThreeComponentsWithMixedEntities(benchmark::State& state) {
    run_iteration<iteration::ThreeComponentSystem<ActiveComponentTypes>>(state, EntityShape::Mixed);
  }

private:
  enum class EntityShape {
    Single,
    Minimal,
    Mixed
  };

  template <typename SystemT>
  void run_iteration(benchmark::State& state, EntityShape shape) {
    using ProbeWorld =
        ::snapecs::World<typename ActiveBenchmarkConfig::BenchmarkComponents, ::snapecs::Systems<SystemT>>;
    using ProbeFactory = entities::EntityFactory<ProbeWorld, ActiveComponentTypes>;

    const auto nentities = static_cast<std::size_t>(state.range(0));
    ProbeWorld world;
    ProbeFactory factory;
    std::vector<::snapecs::Entity> entities;
    entities.reserve(nentities);
    base::ComponentsCounter counters;

    for (std::size_t i = 0, mixed_index = 0; i < nentities; ++i) {
      if (shape == EntityShape::Single) {
        entities.push_back(factory.createSingle(world));
        ++counters.component_one_count;
      } else if (shape == EntityShape::Minimal) {
        entities.push_back(factory.createMinimal(world));
        ++counters.component_one_count;
        ++counters.component_two_count;
      } else {
        entities.push_back(factory.create(world));
        ++counters.component_one_count;
        ++counters.component_two_count;
        ++counters.component_three_count;

        if (nentities < 100 || (i >= 2 * nentities / 4 && i <= 3 * nentities / 4)) {
          if (nentities < 100 || (mixed_index % 10) == 0U) {
            if ((i % 7) == 0U) {
              factory.removeComponentOne(world, entities.back());
              --counters.component_one_count;
            }
            if ((i % 11) == 0U) {
              factory.removeComponentTwo(world, entities.back());
              --counters.component_two_count;
            }
            if ((i % 13) == 0U) {
              factory.removeComponentThree(world, entities.back());
              --counters.component_three_count;
            }
          }
          ++mixed_index;
        }
      }
    }

    world.update();
    for (auto _ : state) {
      world.update();
      benchmark::DoNotOptimize(world.template system<SystemT>().visited());
    }

    this->setCounters(state, entities, counters);
  }
};

#undef ECS_BENCHMARKS_SNAPECS_NAME

} // namespace ecs::benchmarks::snapecs

#endif // ECS_BENCHMARKS_SNAPECS_EXTENDED_BENCHMARK_SUITE_H_
