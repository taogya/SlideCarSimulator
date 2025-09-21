cmake_minimum_required(VERSION 3.10)

project(MotorLib C)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/private/*.c)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Driver)

add_library(MotorLib SHARED ${SOURCES})

target_link_libraries(MotorLib PRIVATE DriverLib)

target_include_directories(MotorLib PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_include_directories(MotorLib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)