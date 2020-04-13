# Copyright 2014-2016, Corvusoft Ltd, All Rights Reserved.

find_path( json_INCLUDE json.hpp HINTS "${CMAKE_SOURCE_DIR}/externals/json/src" "/usr/include" "/usr/local/include" "/opt/local/include" )

if ( json_INCLUDE )
    set( json_FOUND TRUE )

    add_library(json OBJECT ${json_INCLUDE}/json.hpp)
    target_include_directories(json PUBLIC ${json_INCLUDE})

    message( STATUS "${Green}Found JSON include at: ${json_INCLUDE}${Reset}" )
else ( )
    message( FATAL_ERROR "${Red}Failed to locate JSON dependency.${Reset}" )
endif ( )