cmake_minimum_required(VERSION 3.15)

set(HAL_VERSION 1.0.0)

project(hal VERSION ${HAL_VERSION} LANGUAGES CXX)

list(APPEND CMAKE_PREFIX_PATH "install")

find_package(bfc REQUIRED)
find_package(logless REQUIRED) 

if (NOT DEFINED HAL OR HAL STREQUAL "SIM")
    message(STATUS "Using HAL=SIM")
    add_subdirectory(halsim)
endif()
