set(INCLUDE_DIR "include")

function(add_snapecs_extended_benchmark target)
  add_executable(${target} SnapECSExtendedBenchmarkSuite.cpp SnapECSExtendedBenchmarkSuite.h)
  target_include_directories(
    ${target}
    PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>" "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${INCLUDE_DIR}>"
            "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/${INCLUDE_DIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/${INCLUDE_DIR}>")
  target_link_libraries(${target} PRIVATE project_warnings project_options ecs-benchmark ecs-benchmark-example-snapecs)
endfunction()

add_snapecs_extended_benchmark(ecs-benchmark-snapecs-extended)
add_snapecs_extended_benchmark(ecs-benchmark-snapecs-cached-extended)
target_compile_definitions(ecs-benchmark-snapecs-cached-extended PRIVATE SNAPECS_BENCHMARK_CACHED=1)
