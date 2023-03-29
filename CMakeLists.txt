cmake_minimum_required(VERSION 3.5)

project(base64 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -pedantic)
endif()

set(PROJECT_SOURCES
    main.cpp
)

add_executable(base64 main.cpp)

install(TARGETS base64 RUNTIME DESTINATION bin)
