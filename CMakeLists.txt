
cmake_minimum_required (VERSION 3.25)

project (svision2)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(FetchContent)
include(cmake/conan.cmake)
include(FetchContent)

conan_cmake_autodetect(settings)
conan_cmake_run(CONANFILE conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

find_package(fmt CONFIG REQUIRED)

set(SVISION_SOURCES
    src/bitmap.h
    src/bitmap.cpp
    src/platform.cpp
    src/platform.h
)

if (LINUX)
    list(APPEND SVISION_SOURCES
        src/platformx11.cpp
        src/platformx11.h
    )
elseif(WIN32)
    list(APPEND SVISION_SOURCES
        src/platformwin32.cpp
        src/platformwin32.h
    )
endif()
add_library(svision2 ${SVISION_SOURCES})

if (LINUX)
target_link_libraries(svision2 PUBLIC fmt::fmt X11)
else()
target_link_libraries(svision2 PUBLIC fmt::fmt)
endif()
target_include_directories(svision2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable (demo src/main.cxx )
target_link_libraries(demo svision2)
