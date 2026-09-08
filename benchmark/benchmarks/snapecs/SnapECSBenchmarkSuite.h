#ifndef ECS_BENCHMARKS_SNAPECS_BENCHMARK_SUITE_H_
#define ECS_BENCHMARKS_SNAPECS_BENCHMARK_SUITE_H_

#include "ECSBenchmark.h"
#include "benchmark_adapter/Application.h"
#include "benchmark_adapter/EntityFactory.h"
#include "benchmark_adapter/HeroMonsterEntityFactory.h"
#include "benchmark_adapter/Worlds.h"

#include <utility>

namespace ecs::benchmarks::snapecs {

using ActiveBenchmarkConfig = SelectedBenchmarkConfig;
using ActiveApplicationWorld = ActiveBenchmarkConfig::ApplicationWorld;
using ActiveComponentTypes = ActiveBenchmarkConfig::ComponentTypes;
using ActiveSystemTypes = ActiveBenchmarkConfig::SystemTypes;

using SnapECSApplication = Application<ActiveApplicationWorld, ActiveSystemTypes>;
using ApplicationEntityFactory = entities::EntityFactory<ActiveApplicationWorld, ActiveComponentTypes>;
using ApplicationHeroMonsterEntityFactory =
    entities::HeroMonsterEntityFactory<ActiveApplicationWorld, ActiveComponentTypes>;

#if defined(SNAPECS_BENCHMARK_CACHED)
#define ECS_BENCHMARKS_SNAPECS_NAME "snapecs-cached"
#else
#define ECS_BENCHMARKS_SNAPECS_NAME "snapecs-indexed"
#endif

class SnapECSBenchmarkSuite final
    : public base::ECSBenchmark<ECS_BENCHMARKS_SNAPECS_NAME, SnapECSApplication, ApplicationEntityFactory,
                                ApplicationHeroMonsterEntityFactory> {
public:
  SnapECSBenchmarkSuite() = default;

  explicit SnapECSBenchmarkSuite(base::ESCBenchmarkOptions options) : ECSBenchmark(std::move(options)) {}
};

#undef ECS_BENCHMARKS_SNAPECS_NAME

} // namespace ecs::benchmarks::snapecs

#endif // ECS_BENCHMARKS_SNAPECS_BENCHMARK_SUITE_H_
