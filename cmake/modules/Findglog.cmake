find_path(glog_SOURCE CMakeLists.txt HINTS "${CMAKE_SOURCE_DIR}/externals/glog")

if (glog_SOURCE)
    set(glog_FOUND TRUE)
    set(glog_BUILD "${CMAKE_CURRENT_BINARY_DIR}/glog_build")
    set(glog_DISTRIBUTION "${CMAKE_CURRENT_BINARY_DIR}/glog_distribution")

    include(ExternalProject)
    ExternalProject_Add(glog SOURCE_DIR ${glog_SOURCE}
            PREFIX glog_build
            INSTALL_DIR ${glog_DISTRIBUTION}
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${glog_DISTRIBUTION}
            -DBUILD_SHARED=${BUILD_SHARED_LIBS} -DBUILD_TESTING=NO)

    set(glog_INCLUDE "${glog_DISTRIBUTION}/include")

    if (MSVC)

        set(GLOG_LIBRARY_RELEASE ${glog_DISTRIBUTION}/Release/${CMAKE_STATIC_LIBRARY_PREFIX}glog${CMAKE_STATIC_LIBRARY_SUFFIX})

        set(GLOG_LIBRARY_RELEASE ${glog_DISTRIBUTION}/Debug/${CMAKE_STATIC_LIBRARY_PREFIX}glog${CMAKE_STATIC_LIBRARY_SUFFIX})

        set(glog_LIBRARY optimized ${GLOG_LIBRARY_RELEASE} debug ${GLOG_LIBRARY_DEBUG})
    else ()
        # Detect x64 or x32
        set(bitness 32)
        if (CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(bitness 64)
        endif ()
        # Set path
        if (bitness EQUAL 64)
            set(glog_LIBRARY ${glog_DISTRIBUTION}/lib64/${CMAKE_STATIC_LIBRARY_PREFIX}glog${CMAKE_STATIC_LIBRARY_SUFFIX})
        else ()
            set(glog_LIBRARY ${glog_DISTRIBUTION}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glog${CMAKE_STATIC_LIBRARY_SUFFIX})
        endif ()
    endif ()

    message(STATUS "${Green}Found Glog include at: ${glog_SOURCE}${Reset}")
else ()
    message(FATAL_ERROR "${Red}Failed to locate Glog dependency.${Reset}")
endif ()