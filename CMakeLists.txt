cmake_minimum_required(VERSION 3.14)
project(scsc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(common
  src/common/sockethandle.cpp
  src/common/ipv4.cpp
  src/common/port.cpp
  src/common/serveraddress.cpp
)
target_include_directories(common PUBLIC src/common)

add_executable(server
  src/server/server.cpp
  src/server/echoserver.cpp
  src/server/clientconnection.cpp
  src/server/clientaddress.cpp
)
target_include_directories(server PRIVATE src/server)
target_link_libraries(server PRIVATE common)

add_executable(client
  src/client/client.cpp
  src/client/echoclient.cpp
)
target_include_directories(client PRIVATE src/client)
target_link_libraries(client PRIVATE common)

