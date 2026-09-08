find_package(boost_mp11 CONFIG REQUIRED)

add_library(ecs-benchmark-example-snapecs INTERFACE)
add_library(ecs-benchmark::example::snapecs ALIAS ecs-benchmark-example-snapecs)

target_include_directories(
  ecs-benchmark-example-snapecs
  INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>" "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
            "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>")
target_link_libraries(ecs-benchmark-example-snapecs INTERFACE ecs-benchmark-base Boost::mp11)
