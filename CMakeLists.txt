cmake_minimum_required(VERSION 3.19)
project(VisualNovalScripter LANGUAGES C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Lua)
add_subdirectory(LuaBridge)
add_subdirectory(src)
