
cmake_minimum_required(VERSION 3.0.0)
project(kv-resource-pack VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

set(BUILD_SHARED_LIBRARY OFF)
set(BUILD_BULLET OFF)
set(BUILD_OPENGL OFF)
set(BUILD_VULKAN OFF)
set(BUILD_DIRECTX OFF)

add_subdirectory(
	../../kvasir 
	kvasir-engine
)

include_directories(
	../../kvasir
)
add_executable(
	kv-res-pack
	krc-packer.cpp
	main.cpp
)
target_link_libraries(
	kv-res-pack
	kvasir-engine
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
