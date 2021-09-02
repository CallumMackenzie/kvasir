cmake_minimum_required(VERSION 3.0.0)
project(text-rendering-cpp VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (WIN32)
	set(FREETYPE_INCLUDE_DIRS_ ./freetype/include)
	set(FREETYPE_LIBRARIES_ freetype)
	set(FREETYPE_LINK_DIRS_ ./freetype)

	# get_filename_component(FREETYPE_INCLUDE_DIRS_ ./freetype/include REALPATH CMAKE_CURRENT_SOURCE_DIR)
	# get_filename_component(FREETYPE_LIBRARIES_ ./freetype/freetype REALPATH CMAKE_CURRENT_SOURCE_DIR)
	# get_filename_component(FREETYPE_LINK_DIRS_ ./freetype REALPATH CMAKE_CURRENT_SOURCE_DIR)

	# set(FREETYPE_INCLUDE_DIRS ${FREETYPE_INCLUDE_DIRS_} PARENT_SCOPE)
	# set(FREETYPE_LIBRARIES ${FREETYPE_LIBRARIES_} PARENT_SCOPE)
	# set(FREETYPE_LINK_DIRS ${FREETYPE_LINK_DIRS_} PARENT_SCOPE)
else()
	find_package(Freetype REQUIRED)
endif()

include_directories(
	${FREETYPE_INCLUDE_DIRS_}
)

link_directories(
	${FREETYPE_LINK_DIRS_}
)

add_library(kv-text text-img-render.cpp)
target_link_libraries(kv-text ${FREETYPE_LIBRARIES_})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
