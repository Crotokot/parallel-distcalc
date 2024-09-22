cmake_minimum_required(VERSION 3.1)

project(application)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wpedantic")

option(SANITIZE_BUILD "Use sanitizer" OFF)
option(VALGRIND_BUILD "Use valgrind" OFF)
option(LIBSEQ_COMPILE "Build with libseq" ON)
option(LIBPAR_COMPILE "Build with libpar" ON)

if (SANITIZE_BUILD)
    message("Use sanitizer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined -fno-sanitize-recover=all -fsanitize-undefined-trap-on-error -coverage -lgcov -g -Wall -Werror")
    set(CMAKE_CXX_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-sanitize-recover=all -O0")
endif()
if (VALGRIND_BUILD)
    message("use valgrind")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -g -Wall -Werror")
    set(CMAKE_CXX_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -O0")
endif()

if (LIBSEQ_COMPILE)
    add_definitions(-DUSE_LIBSEQ)
endif()
if (LIBPAR_COMPILE)
    add_definitions(-DUSE_LIBPAR)
endif()

add_subdirectory(distcalc)
include_directories(${DISTCALC_INCLUDE_DIRS})
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${DISTCALC_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DISTCALC_LIBRARIES})

enable_testing()
add_subdirectory(tests)
