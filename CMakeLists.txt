cmake_minimum_required(VERSION 3.16)
project(MaxDartsInCircle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(MaxDartsInCircle
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
)

target_link_libraries(MaxDartsInCircle PRIVATE Qt6::Widgets)
