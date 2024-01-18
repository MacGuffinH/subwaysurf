cmake_minimum_required(VERSION 3.26)
project(SubwaySurfer)

set(CMAKE_C_STANDARD 23)
add_executable(mg_subway mg_subway.c)
target_link_libraries(mg_subway SDL2main)
target_link_libraries(mg_subway SDL2)
target_link_libraries(mg_subway SDL2_image)
target_link_libraries(mg_subway SDL2_ttf)
