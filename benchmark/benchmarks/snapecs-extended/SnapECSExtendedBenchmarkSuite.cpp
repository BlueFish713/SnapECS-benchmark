#include "SnapECSExtendedBenchmarkSuite.h"

static ecs::benchmarks::snapecs::SnapECSExtendedBenchmarkSuite benchmark_suite(
    {.add_more_complex_system = ecs::benchmarks::base::add_more_complex_system_t::UseMoreComplexSystems});

ECS_COMPLEX_UPDATE_SYSTEMS_BENCHMARKS(benchmark_suite)
ECS_COMPLEX_FIRST_UPDATE_SYSTEMS_BENCHMARKS(benchmark_suite)
ECS_ITERATE_COMPONENTS_BENCHMARKS(benchmark_suite)
