cmake_minimum_required(VERSION 3.1)
project(sequential)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
if(SANITIZE_BUILD OR VALGRIND_BUILD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -fPIC -O0")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )
endif()

file(GLOB SOURCES *.cpp)
include_directories(${PARENT_DIR} ${PROJECT_SOURCE_DIR})

add_library(${PROJECT_NAME} STATIC ${SOURCES} $<TARGET_OBJECTS:distcalc>)

get_target_property(INCLUDE_DIRS ${PROJECT_NAME} INCLUDE_DIRECTORIES)
set(LIBSEQ_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)
set(LIBSEQ_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)