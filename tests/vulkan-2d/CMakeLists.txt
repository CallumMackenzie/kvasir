
cmake_minimum_required(VERSION 3.0.0)
project(kvasir-tests-vulkan-2d VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

set(BUILD_SHARED_LIBRARY OFF)
set(BUILD_TERMINAL OFF)
set(BUILD_OPENGL OFF)
set(BUILD_FREETYPE OFF)
set(BUILD_BULLET OFF)

add_subdirectory(
	../../kvasir
	kvasir-engine
)

include_directories(
	../../kvasir
)

add_executable(
	kv-vulkan-2d
	./main.cpp
)

target_link_libraries(
	kv-vulkan-2d
	kvasir-engine
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
