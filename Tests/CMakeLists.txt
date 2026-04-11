cmake_minimum_required(VERSION 3.15)
project(TestPackUnpack)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core)

set(SOURCES
    test_pack_unpack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Client/packetbuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Server/packetparser.cpp
)

add_executable(test_pack_unpack ${SOURCES})

target_include_directories(test_pack_unpack PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../common
    ${CMAKE_CURRENT_SOURCE_DIR}/../Client
    ${CMAKE_CURRENT_SOURCE_DIR}/../Server
)

target_link_libraries(test_pack_unpack Qt5::Core)
