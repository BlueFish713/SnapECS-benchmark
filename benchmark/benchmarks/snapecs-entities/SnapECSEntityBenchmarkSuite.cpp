#include "SnapECSEntityBenchmarkSuite.h"

static ecs::benchmarks::snapecs::SnapECSEntityBenchmarkSuite benchmark_suite;

MINIMAL_ECS_ENTITY_BENCHMARKS(benchmark_suite)

static void BM_AddComponent(benchmark::State& state) {
  benchmark_suite.BM_AddComponent(state);
}
BENCHMARK(BM_AddComponent)->Apply(ecs::benchmarks::base::BEDefaultArguments);

static void BM_RemoveAddComponent(benchmark::State& state) {
  benchmark_suite.BM_RemoveAddComponent(state);
}
BENCHMARK(BM_RemoveAddComponent)->Apply(ecs::benchmarks::base::BEDefaultArguments);
