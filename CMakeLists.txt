cmake_minimum_required(VERSION 3.14)
project(scsc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(server src/server.cpp src/sockethandle.cpp src/port.cpp)
add_executable(client src/client.cpp src/sockethandle.cpp src/ipv4address.cpp src/port.cpp)

