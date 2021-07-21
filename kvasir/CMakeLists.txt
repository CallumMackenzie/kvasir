cmake_minimum_required(VERSION 3.0.0)
project(kvasir-engine VERSION 0.1.0)

include(CTest)
enable_testing()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if (WIN32)
	set(FREETYPE_INCLUDE_DIRS ../global/freetype/include)
	set(FREETYPE_LIBRARY freetype)
endif()

add_subdirectory(./lib/glfw glfw)
find_package(Freetype REQUIRED)
link_directories(./lib/glfw ./lib/freetype)

include_directories(
	./src 
	./src/utils 
	./lib/glad/include/glad
	./lib/glfw/include/GLFW
	./lib/glad/include/KHR
	./lib/galg
	${FREETYPE_INCLUDE_DIRS}
)
add_library(kvasir-engine ./src/kvasir-engine.cpp ./lib/glad/glad.c)
target_link_libraries(kvasir-engine glfw ${FREETYPE_LIBRARIES} ) 

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
