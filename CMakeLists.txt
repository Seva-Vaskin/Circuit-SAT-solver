cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)

project(CircuitSATSolver LANGUAGES C CXX)
set(This CircuitSATSolver)


set(ENABLE_TESTING ON CACHE BOOL "Testing for cryptominisat")

add_subdirectory(third_party/cryptominisat)

file(GLOB_RECURSE ${This}-sources
        "src/*.h"
        "src/*.cpp")

add_executable(${This} ${${This}-sources})

target_link_libraries(${This} cryptominisat5)
