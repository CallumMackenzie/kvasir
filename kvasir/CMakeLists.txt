
# Vulkan needs to be installed
# The BULLET_HOME variable needs to be set

cmake_minimum_required(VERSION 3.0.0)
project(kvasir-engine VERSION 0.1.0)

include(CTest)
enable_testing()

if(CMAKE_VERSION VERSION_LESS "3.1")
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
	endif()
else()
	set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if(WIN32)
	set(FREETYPE_INCLUDE_DIRS ./lib/freetype/include)
	set(FREETYPE_LIBRARY freetype)
	set(VULKAN_LINK_DIRS $ENV{VULKAN_SDK/}Lib)
	set(VULKAN_INCLUDE_DIRS $ENV{VULKAN_SDK}/include)
endif()

if(WIN32)
	set(BULLET_ROOT E:/bullet3)
endif()

# set(BUILD_BULLET3 1)
add_subdirectory(./lib/glfw glfw)
add_subdirectory(./lib/bullet3 bullet3)
find_package(Freetype REQUIRED)
find_package(Vulkan REQUIRED FATAL_ERROR)

link_directories(
	./lib/glfw
	./lib/bullet3
	./lib/freetype
	$ENV{LD_LIBRARY_PATH}
	${Vulkan_LIBRARY}
)

include_directories(
	./lib/glad/include/glad
	./lib/glad/include/KHR
	./lib/glfw/include/GLFW
	./lib/galg
	./lib/lodepng
	./src/render-base
	./src/renderers
	./src/world
	./src 
	./src/utils 
	./src/physics
	${FREETYPE_INCLUDE_DIRS}
	${Vulkan_INCLUDE_DIRS}
	./lib/bullet3/src
)

add_library(
	kvasir-engine
	./lib/glad/glad.c 
	./lib/lodepng/lodepng.cpp
	./src/world/position.cpp
	./src/utils/fps-manager.cpp
	./src/render-base/buffer.cpp
	./src/render-base/render-buffer.cpp
	./src/render-base/glfw-window.cpp
	./src/utils/input.cpp
	./src/render-base/texture.cpp
	./src/world/material.cpp
	./src/world/mesh.cpp
	./src/physics/physics.cpp
	./src/physics/bullet-physics3d.cpp
	./src/world/camera.cpp
	./src/render-base/shader.cpp
	./src/renderers/renderer.cpp
	./src/renderers/gl-renderer.cpp
	./src/renderers/vulkan-renderer.cpp
	./src/renderers/terminal-renderer.cpp
	./src/world/game-object.cpp
	./src/utils/common-ops.cpp
	./src/kvasir-engine.cpp 
)
target_link_libraries(
	kvasir-engine 
	glfw  
	${Vulkan_LIBRARIES} 
	BulletInverseDynamics
	BulletSoftBody
	BulletDynamics
	BulletCollision
	LinearMath
	Bullet3Common
	Bullet3Geometry
	BulletSoftBody
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)