#include "SnapECSBenchmarkSuite.h"

static ecs::benchmarks::snapecs::SnapECSBenchmarkSuite benchmark_suite;

ECS_UPDATE_SYSTEMS_BENCHMARKS(benchmark_suite)
ECS_FIRST_UPDATE_SYSTEMS_BENCHMARKS(benchmark_suite)
