cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 11)

project(demo_travis_cmake_gtest_codecov)

# Setup externals
add_subdirectory(externals)

# Find source code
file(GLOB_RECURSE src_files "src/*.c" "src/*.cc" "src/*.cpp" "src/*.h" "src/*.hpp")

add_executable(demo_executable ${src_files})
add_coverage(demo_executable)

# Unit Test
enable_testing()

option(BUILD_TEST "Build the unit tests" ON)
if(BUILD_TEST)
    add_subdirectory(test)
endif()

# evaluate coverage
coverage_evaluate()
