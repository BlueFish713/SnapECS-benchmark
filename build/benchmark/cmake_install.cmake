# Install script for directory: C:/dev/Workspace/Cpp/ecs_benchmark2/benchmark

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ecs-benchmark")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/msys64/ucrt64/bin/objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/entt-entities/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/entt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/entt-extended/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/entityx-entities/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/entityx/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/entityx-extended/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/ginseng-entities/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/ginseng/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/ginseng-extended/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/mustache-entities/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/mustache/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/mustache-extended/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/openecs-entities/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/openecs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/openecs-extended/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/flecs-entities/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/flecs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/flecs-extended/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/pico-ecs-entities/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/pico-ecs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/pico-ecs-extended/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/gaia-ecs-entities/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/gaia-ecs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/gaia-ecs-extended/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/snapecs-entities/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/snapecs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/snapecs-extended/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/benchmarks/oop/cmake_install.cmake")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/dev/Workspace/Cpp/ecs_benchmark2/build/benchmark/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
