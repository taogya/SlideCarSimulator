cmake_minimum_required(VERSION 3.10)

project(SlideCarApp C)

# DEBUGフラグを設定
# add_definitions(-DUARTDRV_DEBUG -DMOTOR_DEBUG)

# サブディレクトリを追加
add_subdirectory(${CMAKE_SOURCE_DIR}/Driver)
add_subdirectory(${CMAKE_SOURCE_DIR}/Motor)
add_subdirectory(
    ${CMAKE_SOURCE_DIR}/../../Protobuf
    ${CMAKE_BINARY_DIR}/Protobuf
)

# main.c を追加
add_executable(SlideCar main.c)
set_target_properties(SlideCar PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist
)

# インクルードディレクトリの設定
target_include_directories(SlideCar
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Driver/public
        ${CMAKE_CURRENT_SOURCE_DIR}/Motor/public
        ${CMAKE_CURRENT_SOURCE_DIR}/../../Protobuf/public
        ${CMAKE_CURRENT_SOURCE_DIR}/../../Protobuf/nanopb

)

# ライブラリのリンク
target_link_libraries(SlideCar
    PRIVATE
        MotorLib  # Motorの動的ライブラリ
        DriverLib  # Driverの動的ライブラリ
        ProtobufLib  # protobufの動的ライブラリ
)