cmake_minimum_required(VERSION 3.0.0)
project(kvasir-tests VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

set(GLOBAL_TEST OFF)
set(TERM_TEST OFF)
set(K2D_TEST OFF)
set(SHARED_LIB_TEST OFF)
set(KV_RESOURCE_BUNDLER OFF)
set(FONT_RENDERING OFF)

if (GLOBAL_TEST)
add_subdirectory(./tests/engine-3d-phys)
endif()

if (TERM_TEST)
add_subdirectory(./tests/engine-terminal)
endif()

if (K2D_TEST)
add_subdirectory(./tests/engine-2d)
endif()

if(KV_RESOURCE_BUNDLER)
add_subdirectory(./tools/kv-resource-pack)
endif()

if(FONT_RENDERING)
add_subdirectory(./tests/font-rendering)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
