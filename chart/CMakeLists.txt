cmake_minimum_required(VERSION 3.5)

project(QTshiboqi LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Charts REQUIRED)

add_executable(QTshiboqi main.cpp  widget.cpp socketclient.cpp client.cpp)

target_link_libraries(QTshiboqi PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Charts)
