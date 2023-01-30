cmake_minimum_required(VERSION 3.25.1)

project(ere-dev)

# set cxx standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC src/main.cpp)

add_subdirectory(lib/ere)

add_executable(eret ${SRC})

target_link_libraries(eret PUBLIC ere)
