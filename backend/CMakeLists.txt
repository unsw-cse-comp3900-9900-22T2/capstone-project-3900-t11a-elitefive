cmake_minimum_required(VERSION 3.13)  # CMake version check
project(prototype)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.hpp"
        server.cpp
)
# link pqxx with classic flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

# Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)  # Enable c++17 standard

# linking zlib
find_package(ZLIB REQUIRED)
find_library(USOCKETS_LIB uSockets)

# linking uwebsockets
find_path(UWEBSOCKETS_INCLUDE_DIRS "uwebsockets/App.h")
include_directories(${UWEBSOCKETS_INCLUDE_DIRS})

# linking nlohmann-json
find_package(nlohmann_json CONFIG REQUIRED)
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)
# include_directories(${JSONCPP_INCLUDE_DIRS})
# target_link_libraries(INTERFACE jsoncpp_object jsoncpp_static JsonCpp::JsonCpp)

# linking pqxx
#add_subdirectory(deps/pqxx build-pqxx)
#find_package(libpqxx REQUIRED)

# create server executable
add_executable(server ${all_SRCS})

target_link_libraries(server PRIVATE
  ZLIB::ZLIB
  ${USOCKETS_LIB}
#   pqxx
  nlohmann_json::nlohmann_json
)