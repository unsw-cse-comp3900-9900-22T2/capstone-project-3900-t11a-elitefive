cmake_minimum_required(VERSION 3.13)  # CMake version check
project(prototype)

# Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)  # Enable c++17 standard

#linking zlib
find_package(ZLIB REQUIRED)
find_library(USOCKETS_LIB uSockets)

#linking uwebsockets
find_path(UWEBSOCKETS_INCLUDE_DIRS "uwebsockets/App.h")
include_directories(${UWEBSOCKETS_INCLUDE_DIRS})

# create server executable
add_executable(server server.cpp)

target_link_libraries(server PRIVATE
  ZLIB::ZLIB
  ${USOCKETS_LIB}
)
