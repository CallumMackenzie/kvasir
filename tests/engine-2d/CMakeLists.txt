
cmake_minimum_required(VERSION 3.0.0)
project(kvasir-tests-2d VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

set(BUILD_BULLET OFF)
set(BUILD_SHARED_LIBRARY OFF)

add_subdirectory(
	../../kvasir
	kvasir-engine
)

include_directories(
	../../kvasir
)

add_executable(
	engine-2d
	./main.cpp
)

target_link_libraries(
	engine-2d
	kvasir-engine
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
