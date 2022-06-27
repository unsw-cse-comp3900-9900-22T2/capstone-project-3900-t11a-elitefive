cmake_minimum_required(VERSION 3.13)  # CMake version check
project(prototype)

# link pqxx with classic flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

# Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)  # Enable c++17 standard

# linking zlib
find_package(ZLIB REQUIRED)
find_library(USOCKETS_LIB uSockets)

# linking uwebsockets
find_path(UWEBSOCKETS_INCLUDE_DIRS "uwebsockets/App.h")
include_directories(${UWEBSOCKETS_INCLUDE_DIRS})

# linking pqxx
#add_subdirectory(deps/pqxx build-pqxx)
#find_package(libpqxx REQUIRED)

# create server executable
add_executable(server server.cpp db_functions.cpp)

target_link_libraries(server PRIVATE
  ZLIB::ZLIB
  ${USOCKETS_LIB}
  pqxx
)