cmake_minimum_required(VERSION 3.1)
project(parallel)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -lrt")
if(SANITIZE_BUILD OR VALGRIND_BUILD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -fPIC -O0")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )
endif()

set(CMAKE_INSTALL_RPATH "/usr/local/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
file(GLOB SOURCES libpar.cpp)
find_package(Boost COMPONENTS filesystem REQUIRED)
include_directories(${PARENT_DIR} ${PROJECT_SOURCE_DIR})
add_library(${PROJECT_NAME} SHARED ${SOURCES} $<TARGET_OBJECTS:distcalc>)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
add_executable(process main.cpp)

install(TARGETS parallel
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION /usr/local/lib
        ARCHIVE DESTINATION lib/parallel)
install(TARGETS process DESTINATION /usr/local/bin)
get_target_property(INCLUDE_DIRS ${PROJECT_NAME} INCLUDE_DIRECTORIES)
target_include_directories(process PUBLIC ${INCLUDE_DIRS})
target_link_libraries(process PRIVATE ${PROJECT_NAME})

set(LIBPAR_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)
set(LIBPAR_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
