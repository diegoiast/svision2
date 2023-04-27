
cmake_minimum_required (VERSION 3.24.2)

project (svision2)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(FetchContent)
include(cmake/conan.cmake)
include(cmake/CPM.cmake)
include(cmake/CreateResources.cmake)

conan_cmake_autodetect(settings)
conan_cmake_run(CONANFILE conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

create_resources("src/*.bin"  "fontdos.h")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SVISION_SOURCES
    src/bitmap.h
    src/bitmap.cpp
    src/widget.h
    src/widget.cpp
    src/button.h
    src/button.cpp
    src/platform.cpp
    src/platform.h
)

if (LINUX)
    list(APPEND SVISION_SOURCES
        src/platformx11.cpp
        src/platformx11.h
    )
    set(SVISION_PLATFORM_LIBS X11)
elseif(WIN32)
    list(APPEND SVISION_SOURCES
        src/platformwin32.cpp
        src/platformwin32.h
    )
endif()
add_library(svision2 ${SVISION_SOURCES})

target_link_libraries(svision2 PUBLIC 
    spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
    ${SVISION_PLATFORM_LIBS})
target_include_directories(svision2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable (demo src/main.cxx )
target_link_libraries(demo svision2)
