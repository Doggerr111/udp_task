cmake_minimum_required(VERSION 3.15)
project(Client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Network)

set(SOURCES
    main.cpp
    packetbuilder.cpp
    udpclient.cpp
    clientcontroller.cpp
    clientwindow.cpp
)

set(HEADERS
    packetbuilder.h
    udpclient.h
    clientcontroller.h
    clientwindow.h
)

add_executable(Client ${SOURCES} ${HEADERS} clientwindow.ui)

target_include_directories(Client PRIVATE ${CMAKE_SOURCE_DIR}/common)

target_link_libraries(Client
    Qt5::Widgets
    Qt5::Network
)
