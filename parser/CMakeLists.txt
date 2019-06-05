cmake_minimum_required (VERSION 2.8)
project (Lex)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(${PROJECT_NAME} 
parser.cpp
exprLex.cpp
main.cpp)
