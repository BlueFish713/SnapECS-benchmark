#ifndef ECS_BENCHMARKS_SNAPECS_ENTITY_BENCHMARK_SUITE_H_
#define ECS_BENCHMARKS_SNAPECS_ENTITY_BENCHMARK_SUITE_H_

#include "EntityBenchmark.h"
#include "benchmark_adapter/EntityFactory.h"
#include "benchmark_adapter/Worlds.h"

#include <utility>

namespace ecs::benchmarks::snapecs {

using ActiveBenchmarkConfig = SelectedBenchmarkConfig;
using ActiveEntityWorld = ActiveBenchmarkConfig::EntityWorld;
using ActiveComponentTypes = ActiveBenchmarkConfig::ComponentTypes;
using SnapECSEntityFactory = entities::EntityFactory<ActiveEntityWorld, ActiveComponentTypes>;

#if defined(SNAPECS_BENCHMARK_CACHED)
#define ECS_BENCHMARKS_SNAPECS_NAME "snapecs-cached"
#else
#define ECS_BENCHMARKS_SNAPECS_NAME "snapecs-indexed"
#endif

class SnapECSEntityBenchmarkSuite final
    : public base::EntityBenchmark<ECS_BENCHMARKS_SNAPECS_NAME, SnapECSEntityFactory> {
public:
  SnapECSEntityBenchmarkSuite() = default;

  explicit SnapECSEntityBenchmarkSuite(base::ESCBenchmarkOptions options) : EntityBenchmark(std::move(options)) {}
};

#undef ECS_BENCHMARKS_SNAPECS_NAME

} // namespace ecs::benchmarks::snapecs

#endif // ECS_BENCHMARKS_SNAPECS_ENTITY_BENCHMARK_SUITE_H_
