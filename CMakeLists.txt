cmake_minimum_required(VERSION 3.16)
project(dcode LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main
    src/parser/parser_registry.cpp
    src/errors/error_registry.cpp
    src/lexer/lexer_registry.cpp
)

target_include_directories(main PRIVATE
    src/errors
    src/lexer
    src/libs
    src/parser
    src/parser/flags
    src/parser/tokens
)

target_link_options(main PRIVATE
    -static-libgcc
    -static-libstdc++
)
