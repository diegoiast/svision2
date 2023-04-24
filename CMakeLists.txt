
cmake_minimum_required (VERSION 3.14)

project (svision2)


set(SVISION_SOURCES
    src/bitmap.h
    src/bitmap.cpp
    src/platform.cpp
    src/platform.h
)

if (linux)
    list(APPEND SVISION_SOURCES
        src/platformx11.cpp
        src/platformx11.h
    )
    target_link_libraries(svision2 X11)
elseif(WIN32)
    list(APPEND SVISION_SOURCES
        src/platformwin32.cpp
        src/platformwin32.h
    )
endif()
add_library(svision2 ${SVISION_SOURCES})

target_include_directories(svision2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable (demo src/main.cxx )
target_include_directories(demo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(demo svision2)
