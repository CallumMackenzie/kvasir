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

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if(WIN32)
	set(FREETYPE_INCLUDE_DIRS ./lib/freetype/include)
	set(FREETYPE_LIBRARY freetype)
	set(VULKAN_LINK_DIRS $ENV{VULKAN_SDK/}Lib)
	set(VULKAN_INCLUDE_DIRS $ENV{VULKAN_SDK}/include)
endif()

add_subdirectory(./lib/glfw glfw)
find_package(Freetype REQUIRED)
find_package(Vulkan REQUIRED FATAL_ERROR)

link_directories(
	./lib/glfw
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
	${FREETYPE_INCLUDE_DIRS},
	${Vulkan_INCLUDE_DIRS}
)

add_library(
	kvasir-engine 
	./lib/glad/glad.c 
	./lib/lodepng/lodepng.cpp
	./src/world/position.cpp
	./src/utils/fps-manager.cpp
	./src/render-base/buffer.cpp
	./src/render-base/glfw-window.cpp
	./src/render-base/texture.cpp
	./src/world/material.cpp
	./src/world/mesh.cpp
	./src/world/camera.cpp
	./src/render-base/shader.cpp
	./src/renderers/renderer.cpp
	./src/renderers/gl-renderer.cpp
	./src/renderers/vulkan-renderer.cpp
	./src/kvasir-engine.cpp 
)
target_link_libraries(kvasir-engine glfw ${Vulkan_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)