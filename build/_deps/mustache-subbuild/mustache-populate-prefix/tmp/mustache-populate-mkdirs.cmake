# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/mustache-src")
  file(MAKE_DIRECTORY "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/mustache-src")
endif()
file(MAKE_DIRECTORY
  "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/mustache-build"
  "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/mustache-subbuild/mustache-populate-prefix"
  "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/mustache-subbuild/mustache-populate-prefix/tmp"
  "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/mustache-subbuild/mustache-populate-prefix/src/mustache-populate-stamp"
  "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/mustache-subbuild/mustache-populate-prefix/src"
  "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/mustache-subbuild/mustache-populate-prefix/src/mustache-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/mustache-subbuild/mustache-populate-prefix/src/mustache-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/mustache-subbuild/mustache-populate-prefix/src/mustache-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
