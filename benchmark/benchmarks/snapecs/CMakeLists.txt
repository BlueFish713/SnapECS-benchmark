set(INCLUDE_DIR "include")

function(add_snapecs_benchmark target)
  add_executable(${target} SnapECSBenchmarkSuite.cpp SnapECSBenchmarkSuite.h)
  target_include_directories(
    ${target}
    PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>" "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${INCLUDE_DIR}>"
            "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/${INCLUDE_DIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/${INCLUDE_DIR}>")
  target_link_libraries(${target} PRIVATE project_warnings project_options ecs-benchmark ecs-benchmark-example-snapecs)
endfunction()

add_snapecs_benchmark(ecs-benchmark-snapecs)
add_snapecs_benchmark(ecs-benchmark-snapecs-cached)
target_compile_definitions(ecs-benchmark-snapecs-cached PRIVATE SNAPECS_BENCHMARK_CACHED=1)
