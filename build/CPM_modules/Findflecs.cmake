include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/cmake/CPM_0.40.2.cmake")
CPMAddPackage("NAME;flecs;VERSION;4.0.1;GITHUB_REPOSITORY;SanderMertens/flecs;OPTIONS;FLECS_STATIC ON;FLECS_SHARED OFF")
set(flecs_FOUND TRUE)