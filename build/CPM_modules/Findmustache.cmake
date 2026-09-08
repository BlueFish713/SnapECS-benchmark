include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/cmake/CPM_0.40.2.cmake")
CPMAddPackage("NAME;mustache;VERSION;0.2;GITHUB_REPOSITORY;kirillochnev/mustache;GIT_TAG;master;OPTIONS;MUSTACHE_CXX_STD cxx_std_20;MUSTACHE_USE_SANITIZER OFF;MUSTACHE_WARNINGS_AS_ERROR OFF;MUSTACHE_BUILD_SHARED OFF")
set(mustache_FOUND TRUE)