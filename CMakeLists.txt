cmake_minimum_required (VERSION 3.22.0)

project (svision2)

option(SVISION_USE_STB "Use STB library" ON)
option(SVISION_USE_FREETYPE "Use FreeType library" OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(FetchContent)
include(cmake/conan.cmake)
include(cmake/CPM.cmake)
include(cmake/CreateResources.cmake)

# https://github.com/heavyai/heavydb/blob/72c90bc290b79dd30240da41c103a00720f6b050/CMakeLists.txt#L119C1-L140C8
macro(set_alternate_linker linker)
  find_program(LINKER_EXECUTABLE ld.${USE_ALTERNATE_LINKER} ${USE_ALTERNATE_LINKER})
  if(LINKER_EXECUTABLE)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 12.0.0)
      add_link_options("-ld-path=${USE_ALTERNATE_LINKER}")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 12.1.0 AND "${USE_ALTERNATE_LINKER}" STREQUAL "mold")
      # LINKER_EXECUTABLE will be a full path to ld.mold, so we replace the end of the path, resulting in the relative
      # libexec/mold dir, and tell GCC to look there first for an override version of executables, in this case, ld
      string(REPLACE "bin/ld.mold" "libexec/mold" PATH_TO_LIBEXEC_MOLD ${LINKER_EXECUTABLE})
      add_link_options("-B${PATH_TO_LIBEXEC_MOLD}")
    else()
      add_link_options("-fuse-ld=${USE_ALTERNATE_LINKER}")
    endif()
  else()
    set(USE_ALTERNATE_LINKER "" CACHE STRING "Use alternate linker" FORCE)
  endif()
endmacro()

set(USE_ALTERNATE_LINKER "mold" CACHE STRING "Use alternate linker. Leave empty for system default; alternatives are 'gold', 'lld', 'bfd', 'mold'")
if(NOT "${USE_ALTERNATE_LINKER}" STREQUAL "")
  set_alternate_linker(${USE_ALTERNATE_LINKER})
endif()

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
find_package(freetype CONFIG REQUIRED)
find_package(Catch2 3 REQUIRED)

create_resources("src/*.bin"  "generated_headers/fontdos.h")
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated_headers)


# Main library
set(SVISION_SOURCES
    src/bitmap.cpp
    src/bitmap.h
    src/button.cpp
    src/button.h
    src/buttonstates.cpp
    src/buttonstates.h
    src/combobox.cpp
    src/combobox.h
    src/checkbox.cpp
    src/checkbox.h
    src/checkboxshape.h
    src/fontprovider.cpp
    src/fontprovider.h
    src/imageview.cxx
    src/imageview.hpp
    src/label.cpp
    src/label.h
    src/layout.cpp
    src/layout.h
    src/listview.cpp
    src/listview.h
    src/mousecursors.h
    src/platform.cpp
    src/platform.h
    src/radiobuttongroup.h
    src/radiobuttongroup.cpp
    src/scrollbar.cpp
    src/scrollbar.h
    src/spinbox.cpp
    src/spinbox.h
    src/stackwidget.cpp
    src/stackwidget.h
    src/tabheader.cpp
    src/tabheader.h
    src/theme.cpp
    src/theme.h
    src/timer.cpp
    src/timer.h
    src/textfield.cpp
    src/textfield.h
    src/tabwidget.cpp
    src/tabwidget.h
    src/toolbar.cxx
    src/toolbar.hpp
    src/widget.cpp
    src/widget.h
    src/image/loaders.cxx
    src/image/loaders.hpp
    src/themes/fluent.cpp
    src/themes/fluent.h
)

if (SVISION_USE_FREETYPE)
    list(APPEND SVISION_SOURCES
        src/fontproviderfreetype.cpp
        src/fontproviderfreetype.h
    )
endif()

if (SVISION_USE_STB)
    list(APPEND SVISION_SOURCES
        src/fontproviders/fontproviderstb.cxx
        src/fontproviders/fontproviderstb.hpp
    )
endif()

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

if (SVISION_USE_FREETYPE)
    add_compile_definitions(svision2 -DSVISION_USE_FREETYPE)
endif()

if(SVISION_USE_STB)
   add_compile_definitions(svision2 -DSVISION_USE_STB)
endif()


target_link_libraries(svision2 PUBLIC 
    spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
    freetype
    ${SVISION_PLATFORM_LIBS})
target_include_directories(svision2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(svision2 PUBLIC cxx_std_17)

if(MSVC)
  target_compile_options(svision2 PRIVATE /W4)
  # this shuts up stl4043
  target_compile_definitions(svision2 PRIVATE _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)
else()
  target_compile_options(svision2 PRIVATE -Wall -Wextra)
endif()

# Development demo
add_executable (demo src/main.cxx )
target_link_libraries(demo svision2)
add_custom_command(TARGET demo PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/vampire-riding-a-dinozaur.png"
    "${CMAKE_BINARY_DIR}/vampire-riding-a-dinozaur.png"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/vampire-riding-a-dinozaur.png"
    COMMENT "Link image to build dir"
)

# Create a GUI application (no console)
set_target_properties(demo PROPERTIES WIN32_EXECUTABLE TRUE)

# Unit tests
include(CTest)
include(Catch)

add_executable(test-buttons tests/test_buttons.cpp)
target_link_libraries(test-buttons PRIVATE Catch2::Catch2WithMain svision2)
catch_discover_tests(test-buttons)
