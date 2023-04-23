
cmake_minimum_required (VERSION 3.14)

project (svision2)

add_executable (demo src/main.cxx src/platformx11.cpp)
target_include_directories(demo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(demo X11)
