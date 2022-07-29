cmake_minimum_required(VERSION 3.13)  # CMake version check
project(prototype)

include_directories(
        ${PROJECT_SOURCE_DIR}/libs/uWS/
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/game/include/  # Note: Need to say where to find the headers
        ${PROJECT_SOURCE_DIR}/src/server/include/  # Note: Need to say where to find the headers
        ${PROJECT_SOURCE_DIR}/src/database/include/  # Note: Need to say where to find the headers
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
find_library(USOCKETS_LIB uSockets)

# linking uwebsockets
# find_path(UWEBSOCKETS_INCLUDE_DIRS "uwebsockets/App.h")
# include_directories(${UWEBSOCKETS_INCLUDE_DIRS})

# linking nlohmann-json
find_package(nlohmann_json CONFIG REQUIRED)
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)

# create server executable
add_executable(server ${all_SRCS})

target_link_libraries(server PRIVATE
  ZLIB::ZLIB
  ${USOCKETS_LIB}
  pqxx
  crypto
  nlohmann_json::nlohmann_json
)