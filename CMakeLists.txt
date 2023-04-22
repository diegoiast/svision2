
cmake_minimum_required (VERSION 3.14)

project (HELLO)

add_executable (demo src/main.cxx)
target_link_libraries(demo X11)
