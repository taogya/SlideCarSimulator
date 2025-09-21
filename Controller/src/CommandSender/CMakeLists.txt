cmake_minimum_required(VERSION 3.10)

project(CommandSenderLib C)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/private/*.c)

add_library(CommandSenderLib SHARED ${SOURCES})

target_link_libraries(CommandSenderLib PRIVATE DriverLib ProtobufLib ButtonLib)

target_include_directories(CommandSenderLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)