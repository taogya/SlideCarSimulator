cmake_minimum_required(VERSION 3.10)

project(DriverLib C)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/private/*.c)

add_library(DriverLib SHARED ${SOURCES})

target_include_directories(DriverLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)
