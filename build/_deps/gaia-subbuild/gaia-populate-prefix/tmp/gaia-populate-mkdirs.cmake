# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/gaia-src")
  file(MAKE_DIRECTORY "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/gaia-src")
endif()
file(MAKE_DIRECTORY
  "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/gaia-build"
  "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/gaia-subbuild/gaia-populate-prefix"
  "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/gaia-subbuild/gaia-populate-prefix/tmp"
  "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/gaia-subbuild/gaia-populate-prefix/src/gaia-populate-stamp"
  "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/gaia-subbuild/gaia-populate-prefix/src"
  "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/gaia-subbuild/gaia-populate-prefix/src/gaia-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/gaia-subbuild/gaia-populate-prefix/src/gaia-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/dev/Workspace/Cpp/ecs_benchmark2/build/_deps/gaia-subbuild/gaia-populate-prefix/src/gaia-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
