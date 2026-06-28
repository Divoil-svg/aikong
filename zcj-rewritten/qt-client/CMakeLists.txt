cmake_minimum_required(VERSION 3.20)
project(aikong-qt-client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

add_executable(aikong-qt-client
    main.cpp
    MainWindow.cpp
    MainWindow.h
    RemoteDesktopView.cpp
    RemoteDesktopView.h
)

target_link_libraries(aikong-qt-client PRIVATE
    Qt6::Widgets
    Qt6::Network
)

# 未来集成 Rust FFI
# target_include_directories(...)
# target_link_libraries(... aikong_core)