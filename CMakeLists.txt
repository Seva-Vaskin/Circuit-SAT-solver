cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)

project(CircuitSATSolver LANGUAGES CXX)
set(This CircuitSATSolver)

# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -m64 -Wfloat-equal -Wlogical-op -Wall -Wextra -Wno-unused-result -D_GLIBCXX_DEBUG -fsanitize=undefined -fstack-protector-strong -fno-sanitize-recover=all")

# Library
set(ENABLE_TESTING ON CACHE BOOL "Testing for cryptominisat")

add_subdirectory(third_party/cryptominisat)

file(GLOB_RECURSE ${This}-sources
        "lib/*.h"
        "lib/*.cpp")

add_library(${This}-lib ${${This}-sources})

target_link_libraries(${This}-lib cryptominisat5)

target_include_directories(${This}-lib
        PUBLIC lib
        PUBLIC lib/solver
        )

# Executable
add_executable(${This} main.cpp ${${This}-sources})
target_link_libraries(${This} ${This}-lib)

#Tests
add_subdirectory(third_party/catch2)
file(GLOB_RECURSE ${This}-test-sources "tests/*.cpp")
add_executable(${This}-tests ${${This}-test-sources})
target_link_libraries(${This}-tests ${This}-lib)
target_link_libraries(${This}-tests Catch2::Catch2WithMain)
