cmake_minimum_required(VERSION 3.10)

project(ProtobufLib C)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/public/SlideCarCommand.pb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/pb_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/pb_encode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/pb_decode.c
)

add_library(ProtobufLib SHARED ${SOURCES})

target_include_directories(ProtobufLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/nanopb
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)