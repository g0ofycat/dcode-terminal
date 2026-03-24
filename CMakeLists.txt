cmake_minimum_required(VERSION 3.16)
project(dcode LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(dcode
    usage/main.cpp
    src/parser/parser_registry.cpp
    src/errors/error_registry.cpp
    src/lexer/lexer_registry.cpp
)

target_include_directories(dcode PRIVATE
    src/errors
    src/executor
    src/lexer
    src/libs
    src/parser
    src/parser/flags
    src/parser/tokens
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_options(dcode PRIVATE -static-libgcc -static-libstdc++)
endif()
