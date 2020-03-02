cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 11)

project(news_blockchain)

# Setup externals
add_subdirectory(externals)

# Find source code
file(GLOB_RECURSE src_files "src/*.c" "src/*.cc" "src/*.cpp" "src/*.h" "src/*.hpp")

add_executable(news_blockchain ${src_files})
add_coverage(news_blockchain)

# Unit Test
enable_testing()

option(BUILD_TEST "Build the unit tests" ON)
if(BUILD_TEST)
    add_subdirectory(test)
endif()

# evaluate coverage
coverage_evaluate()
