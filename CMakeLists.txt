cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 14)

project(news_blockchain)

# Setup externals
include(externals/CMakeLists.txt)

# Find source code
file(GLOB_RECURSE src_files "src/*.c" "src/*.cc" "src/*.cpp" "src/*.h" "src/*.hpp")
list(REMOVE_ITEM src_files "${CMAKE_SOURCE_DIR}/src/main.cpp")

add_executable(news_blockchain src/main.cpp ${src_files})
target_include_directories(news_blockchain PUBLIC src)

# Dependencies to be builded before main executable
add_dependencies(news_blockchain restbed glog)

# Link
target_link_libraries(news_blockchain
        ${restbed_LIBRARY} Threads::Threads
        json
        ${glog_LIBRARY} ${Unwind_LIBRARY})
target_include_directories(news_blockchain PUBLIC
        ${restbed_INCLUDE}
        ${glog_INCLUDE})

# Coverage
add_coverage(news_blockchain)

# Unit Test
enable_testing()

option(BUILD_TEST "Build the unit tests" ON)
if (BUILD_TEST)
    add_subdirectory(test)
endif ()

# evaluate coverage
coverage_evaluate()
