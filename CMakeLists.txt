cmake_minimum_required (VERSION 3.2.20)
project (svision)


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable (demo
    main.cxx
    surface.cpp
    surface.h
    sdl_surface.cpp
    sdl_surface.h
)
target_link_libraries(demo ${SDL2_LIBRARIES})
#target_link_libraries (demo LINK_PUBLIC Hello)
