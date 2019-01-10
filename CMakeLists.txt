cmake_minimum_required(VERSION 2.6)
file(TO_CMAKE_PATH "$ENV{IMAGINEPP_ROOT}" d)
if(NOT EXISTS "${d}")
    message(FATAL_ERROR "Error: environment variable IMAGINEPP_ROOT=" "${d}")
endif()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${d}/CMake")
find_package(Imagine)

#SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
#SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#find_package(Qt5Multimedia)

project(Pookiemon)
add_executable(Pookiemon main.cpp heros.cpp carte.cpp combat.cpp pokemon.cpp market.cpp)
ImagineUseModules(Pookiemon Graphics)
ImagineUseModules(Pookiemon Images)

# Use the Widgets module from Qt 5.
#qt5_use_modules(Pookiemon Multimedia)
