####################################################################

cmake_minimum_required(VERSION 3.28)

####################################################################

project(06_05 VERSION 24.02.06 LANGUAGES CXX)

####################################################################

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_REQUIRED True)

####################################################################

set(Boost_USE_STATIC_LIBS    ON)

set(Boost_USE_STATIC_RUNTIME ON)

####################################################################

find_package(Boost 1.85 REQUIRED COMPONENTS filesystem system)

####################################################################

include_directories(${Boost_INCLUDE_DIRS})

####################################################################

add_library            (shared_v1 SHARED lib_v1.cpp)

target_compile_options (shared_v1 PRIVATE -Wall -Wextra -Wpedantic -O0)

####################################################################

add_library            (shared_v2 SHARED lib_v2.cpp)

target_compile_options (shared_v2 PRIVATE -Wall -Wextra -Wpedantic -O0)

####################################################################

add_executable             (06_05 main.cpp)

target_compile_options     (06_05 PRIVATE -Wall -Wextra -Wpedantic -O0)

target_link_libraries      (06_05 PRIVATE ${Boost_LIBRARIES})

target_link_libraries      (06_05 PRIVATE dl)

####################################################################
