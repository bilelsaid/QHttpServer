cmake_minimum_required(VERSION 3.14)
project(QHttpServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network HttpServer)

# Ensure that MOC is enabled
set(CMAKE_AUTOMOC ON)

# Create an executable and link it with Qt libraries
add_executable(QHttpServer main.cpp server.cpp server.h)
target_link_libraries(QHttpServer Qt6::Core Qt6::Network Qt6::HttpServer)
