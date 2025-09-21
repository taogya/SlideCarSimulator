cmake_minimum_required(VERSION 3.10)

project(ButtonLib C)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/private/*.c)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Driver)

add_library(ButtonLib SHARED ${SOURCES})

target_link_libraries(ButtonLib PRIVATE DriverLib)

target_include_directories(ButtonLib PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_include_directories(ButtonLib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)