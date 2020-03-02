# Setup Codecov
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/externals/CMake-codecov/cmake" ${CMAKE_MODULE_PATH})
find_package(codecov)

# For unit test
option(BUILD_TEST "Build the unit tests" ON)
if(BUILD_TEST)

    # Google Test
    set(GOOGLETEST_ROOT externals/gtest/googletest CACHE STRING "Google Test source root")

    set(GOOGLETEST_SOURCES
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest-all.cc
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest_main.cc
        )

    foreach(_source ${GOOGLETEST_SOURCES})
        set_source_files_properties(${_source} PROPERTIES GENERATED 1)
    endforeach()

    add_library(gtest ${GOOGLETEST_SOURCES})
    
    target_include_directories(gtest PUBLIC
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
        )

endif()
