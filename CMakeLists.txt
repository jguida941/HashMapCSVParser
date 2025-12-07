cmake_minimum_required(VERSION 3.25)
project(HashMap LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(HashMap
        src/HashTable.cpp
        src/CSVparser.cpp
)

target_include_directories(HashMap PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)