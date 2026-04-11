cmake_minimum_required(VERSION 3.15)
project(Server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Network)

set(SOURCES
    main.cpp
    udpserver.cpp
    limitsreader.cpp
    packetparser.cpp
    servercontroller.cpp
    serverwindow.cpp
)

set(HEADERS
    udpserver.h
    limitsreader.h
    packetparser.h
    servercontroller.h
    serverwindow.h
)

add_executable(Server ${SOURCES} ${HEADERS} serverwindow.ui)

# Копируем limits.json в папку сборки
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/limits.json
               ${CMAKE_CURRENT_BINARY_DIR}/limits.json
               COPYONLY)

target_include_directories(Server PRIVATE ${CMAKE_SOURCE_DIR}/common)

target_link_libraries(Server
    Qt5::Widgets
    Qt5::Network
)
