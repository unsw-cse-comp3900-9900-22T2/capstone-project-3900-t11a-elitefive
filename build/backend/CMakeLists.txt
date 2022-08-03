cmake_minimum_required(VERSION 3.13)  # CMake version check
project(prototype)

# Note: Need to say where to find the headers
include_directories(
        ${PROJECT_SOURCE_DIR}/libs/
        ${PROJECT_SOURCE_DIR}/libs/uSockets/
        ${PROJECT_SOURCE_DIR}/libs/uWS/
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/game/include/  
        ${PROJECT_SOURCE_DIR}/src/server/include/  
        ${PROJECT_SOURCE_DIR}/src/database/include/  
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/server/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/game/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/database/*.cpp"

)
list(REMOVE_ITEM all_SRCS "${PROJECT_SOURCE_DIR}/src/game/main_test.cpp") # Note: This file is just for local testing


# Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)  # Enable c++17 standard

# linking pqxx 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq -lcrypto")

# linking zlib
find_package(ZLIB REQUIRED)

# create server executable
add_executable(server ${all_SRCS})

target_link_libraries(server PRIVATE
  ZLIB::ZLIB
  pqxx
  crypto
  ${PROJECT_SOURCE_DIR}/libs/uSockets/uSockets.a
)