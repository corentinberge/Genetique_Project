cmake_minimum_required(VERSION 3.14)
project(Genetique_Project C)

set(CMAKE_C_STANDARD 99)

add_executable(Genetique_Project main.c)

target_link_libraries(Genetique_Project m)