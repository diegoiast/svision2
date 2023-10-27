cmake_minimum_required (VERSION 3.22.0)

project (svision2)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(FetchContent)
include(cmake/conan.cmake)
include(cmake/CPM.cmake)
include(cmake/CreateResources.cmake)

cpmaddpackage(
    NAME
    Format.cmake
    VERSION
    1.7.2
    GITHUB_REPOSITORY
    TheLartians/Format.cmake
    OPTIONS # set to yes skip cmake formatting
    "FORMAT_SKIP_CMAKE NO"
    # path to exclude (optional, supports regular expressions)
    "CMAKE_FORMAT_EXCLUDE cmake/*"
)


conan_cmake_autodetect(settings)
conan_cmake_run(CONANFILE conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Catch2 3 REQUIRED)

create_resources("src/*.bin"  "fontdos.h")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Main library
set(SVISION_SOURCES
    src/bitmap.cpp
    src/bitmap.h
    src/button.cpp
    src/button.h
    src/buttonstates.cpp
    src/buttonstates.h
    src/checkbox.cpp
    src/checkbox.h
    src/checkboxshape.h
    src/label.cpp
    src/label.h
    src/listview.cpp
    src/listview.h
    src/platform.cpp
    src/platform.h
    src/radiobuttongroup.h
    src/radiobuttongroup.cpp
    src/scrollbar.cpp
    src/scrollbar.h
    src/spinbox.cpp
    src/spinbox.h
    src/theme.cpp
    src/theme.h
    src/timer.cpp
    src/timer.h
    src/textfield.cpp
    src/textfield.h
    src/widget.cpp
    src/widget.h
)

if (UNIX)
    list(APPEND SVISION_SOURCES
        src/platformx11.cpp
        src/platformx11.h
    )
    set(SVISION_PLATFORM_LIBS X11)
elseif(WIN32)
    list(APPEND SVISION_SOURCES
        src/platformwin32.cpp
        src/platformwin32.h
        src/platformwin32-keycodes.h
    )
else()
	message(FATAL_ERROR "unsupported platform" )
endif()
add_library(svision2 ${SVISION_SOURCES})

target_link_libraries(svision2 PUBLIC 
    spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
    ${SVISION_PLATFORM_LIBS})
target_include_directories(svision2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(svision2 PUBLIC cxx_std_17)

# Development demo
add_executable (demo src/main.cxx )
target_link_libraries(demo svision2)
set_target_properties(demo PROPERTIES
    WIN32_EXECUTABLE TRUE  # Create a GUI application (no console)
)

# Unit tests
include(CTest)
include(Catch)

add_executable(test-buttons tests/test_buttons.cpp)
target_link_libraries(test-buttons PRIVATE Catch2::Catch2WithMain svision2)
catch_discover_tests(test-buttons)
