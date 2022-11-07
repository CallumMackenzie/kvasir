
cmake_minimum_required(VERSION 3.0.0)
project(kvasir-tests-3d-phys VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

set(BUILD_SHARED_LIBRARY OFF)
set(BUILD_VULKAN OFF)

add_subdirectory(
	../../kvasir
	kvasir-engine
)

include_directories(
	../../kvasir
)

add_executable(
	engine-3d-phys
	./main.cpp
)

target_link_libraries(
	engine-3d-phys
	kvasir-engine
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
