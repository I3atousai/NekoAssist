cmake_minimum_required(VERSION 3.14)

project(NekoAssist LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)


add_executable(NekoAssist
  main.cpp
  drag.h
  drag.cpp
  drop.h
  drop.cpp
)
qt_add_resources(NekoAssist "images"
     PREFIX "/images"
     FILES neko2.png Neco-Arc_Remake.png )
target_link_libraries(NekoAssist Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(NekoAssist Qt${QT_VERSION_MAJOR}::Network)
