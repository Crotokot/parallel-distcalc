cmake_minimum_required(VERSION 3.1)
project(unittests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
if(SANITIZE_BUILD OR VALGRIND_BUILD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -fPIC -O0")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )
endif()

file(GLOB SOURCES *.cpp)
include_directories(${DISTCALC_INCLUDE_DIRS})

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${DISTCALC_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${DISTCALC_LIBRARIES} GTest::gtest_main)

gtest_discover_tests(${PROJECT_NAME})