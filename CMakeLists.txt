cmake_minimum_required(VERSION 3.0.0)
project(kvasir-tests VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

add_subdirectory(./kvasir kvasir-engine)

include_directories(
	./kvasir
)

set(BUILD_TESTS ON)
set(GLOBAL_TEST ON)
set(TERM_TEST OFF)
set(K2D_TEST OFF)
set(SHARED_LIB_TEST OFF)

if (BUILD_TESTS)

if (GLOBAL_TEST)
add_executable(global-testing ./tests/global-tests.cpp)
target_link_libraries(global-testing kvasir-engine)
endif()

if (TERM_TEST)
add_executable(term-test ./tests/terminal-test.cpp)
target_link_libraries(term-test kvasir-engine)
endif()

if (K2D_TEST)
add_executable(k2d-test ./tests/k2d-tests.cpp)
target_link_libraries(k2d-test kvasir-engine)
endif()

if(SHARED_LIB_TEST)
add_executable(shared-lib-test ./tests/shared-lib-test.cpp)
target_link_libraries(shared-lib-test kvasir-engine)
endif()

endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
