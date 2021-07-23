cmake_minimum_required(VERSION 3.0.0)
project(kvasir-tests VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

add_subdirectory(./kvasir kvasir-engine)

include_directories(
	./kvasir
)

add_executable(global-testing ./tests/global-tests.cpp)
target_link_libraries(global-testing kvasir-engine)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
