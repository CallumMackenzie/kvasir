
# Macros
macro(KVDEFAULT VNAME DEFVAL)
	if (NOT DEFINED ${VNAME})
		set(${VNAME} ${DEFVAL})
		message(${VNAME} " definition not found, defaulting to " ${DEFVAL} ".")
	endif()
endmacro()

# Config
KVDEFAULT(BUILD_OPENGL ON)
KVDEFAULT(BUILD_VULKAN ON)
KVDEFAULT(BUILD_TERMINAL ON)
KVDEFAULT(BUILD_DIRECTX ON)
KVDEFAULT(BUILD_FREETYPE OFF)
KVDEFAULT(BUILD_BULLET ON)
KVDEFAULT(CMAKE_POSITION_INDEPENDENT_CODE ON)
KVDEFAULT(BUILD_SHARED_LIBRARY OFF)

# CMake general
cmake_minimum_required(VERSION 3.1.2)
project(kvasir-engine VERSION 0.1.0)
include(CTest)
enable_testing()

# Use c++ 17
if(CMAKE_VERSION VERSION_LESS "3.1")
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
	endif()
else()
	set(CMAKE_CXX_STANDARD 17)
endif()

# GLFW config
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Windows linking
if(WIN32)
	if (BUILD_VULKAN)
		set(VULKAN_LINK_DIRS $ENV{VULKAN_SDK}/Lib)
		set(VULKAN_INCLUDE_DIRS $ENV{VULKAN_SDK}/include)
	endif()
endif()

# Preprocessor definitions
if(BUILD_VULKAN)
	add_definitions(-DBUILD_VULKAN=1)
endif()
if(BUILD_BULLET)
	add_definitions(-DBUILD_BULLET=1)
endif()
if(BUILD_TERMINAL)
	add_definitions(-DBUILD_TERMINAL=1)
endif()
if(BUILD_OPENGL)
	add_definitions(-DBUILD_OPENGL=1)
endif()
if (BUILD_DIRECTX)
	add_definitions(-DBUILD_DIRECTX=1)
endif()
if (BUILD_FREETYPE)
	add_definitions(-DBUILD_FREETYPE=1)
endif()

# Dependencies
add_subdirectory(./lib/glfw glfw)
if(BUILD_BULLET)
	add_subdirectory(./lib/bullet3 bullet3)
endif()
if (BUILD_FREETYPE)
	add_subdirectory(./lib/kv-text kv-text)
endif()
if (BUILD_VULKAN)
	find_package(Vulkan REQUIRED FATAL_ERROR)
	message("Vulkan found.")
endif()

# Include dirs
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
)
if(BUILD_BULLET)
	include_directories(
		./lib/bullet3/src
	)
endif()
if (BUILD_VULKAN)
	include_directories(
		${Vulkan_INCLUDE_DIRS}
	)
endif()
if (BUILD_FREETYPE)
	# include_directories(
	# 	${FREETYPE_INCLUDE_DIRS}
	# )
endif()

# Files
set(KV_LIB_TYPE)
if(BUILD_SHARED_LIBRARY)
	set(KV_LIB_TYPE SHARED)
	add_definitions(-DBUILD_SHARED=1)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
else()
	add_definitions(-DBUILD_STATIC=1)
endif()
add_library(
	kvasir-engine ${KV_LIB_TYPE}
	./lib/galg/galg_ops.cpp
	./lib/glad/glad.c 
	./lib/lodepng/lodepng.cpp
	./src/world/position.cpp
	./src/utils/fps-manager.cpp
	./src/render-base/buffer.cpp
	./src/render-base/render-buffer.cpp
	./src/render-base/glfw-window.cpp
	./src/utils/input.cpp
	./src/render-base/texture.cpp
	./src/render-base/font-renderer.cpp
	./src/utils/data.cpp
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

# Link dirs
link_directories(
	./lib/glfw
	$ENV{LD_LIBRARY_PATH}
)
if (BUILD_VULKAN)
	link_directories(
		${Vulkan_LIBRARY}
	)
endif()
if (BUILD_BULLET)
	link_directories(
		./lib/bullet3
	)
endif()
if (BUILD_BULLET)
	# link_directories(
	# 	${FREETYPE_LINK_DIRS}
	# )
endif()

# Link libraries
target_link_libraries(
	kvasir-engine 
	glfw 
)
if (BUILD_VULKAN)
	target_link_libraries(
		kvasir-engine
		${Vulkan_LIBRARIES}
	)
endif()
if(BUILD_BULLET)
	target_link_libraries(
		kvasir-engine
		BulletInverseDynamics
		BulletSoftBody
		BulletDynamics
		BulletCollision
		LinearMath
		Bullet3Common
		Bullet3Geometry
		BulletSoftBody
	)
endif()
if (BUILD_FREETYPE)
	target_link_libraries(
		kvasir-engine
		# ${FREETYPE_LIBRARIES}
		kv-text
	)
endif()

# General CMake
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)