cmake_minimum_required(VERSION 3.23)

project(stock_exchange LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(matching_core
    src/matching_core/order_book.cpp
    src/matching_core/matching_engine.cpp
)

target_include_directories(matching_core
    PUBLIC
        include
)
