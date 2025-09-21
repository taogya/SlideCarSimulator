cmake_minimum_required(VERSION 3.10)

project(ControllerApp C)

# DEBUGフラグを設定
# add_definitions(-DTIMERDRV_DEBUG -DBUTTON_DEBUG -DBUTTONDRV_DEBUG -DCOMMAND_DEBUG -DUARTDRV_DEBUG)

# サブディレクトリを追加
add_subdirectory(${CMAKE_SOURCE_DIR}/Driver)
add_subdirectory(${CMAKE_SOURCE_DIR}/Button)
add_subdirectory(${CMAKE_SOURCE_DIR}/CommandSender)
add_subdirectory(
    ${CMAKE_SOURCE_DIR}/../../Protobuf
    ${CMAKE_BINARY_DIR}/Protobuf
)

# main.c を追加
add_executable(Controller main.c)
set_target_properties(Controller PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist
)

# インクルードディレクトリの設定
target_include_directories(Controller
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Driver/public
        ${CMAKE_CURRENT_SOURCE_DIR}/Button/public
        ${CMAKE_CURRENT_SOURCE_DIR}/CommandSender/public
        ${CMAKE_CURRENT_SOURCE_DIR}/../../Protobuf/public
        ${CMAKE_CURRENT_SOURCE_DIR}/../../Protobuf/nanopb
)

# ライブラリのリンク
target_link_libraries(Controller
    PRIVATE
        ButtonLib      # Buttonの静的ライブラリ
        CommandSenderLib     # CommandSenderの静的ライブラリ
        DriverLib  # Driverの動的ライブラリ
        ProtobufLib  # protobufの動的ライブラリ
)