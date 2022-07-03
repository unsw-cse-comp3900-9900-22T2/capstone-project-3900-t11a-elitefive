cmake_minimum_required(VERSION 3.0.0)
project(board VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)  # Enable c++17 standard
# set(CMAKE_BUILD_TYPE Debug)

include_directories(
        ${PROJECT_SOURCE_DIR}/game/include/  # Note: Need to say where to find the headers
)
# # Local header files here ONLY
# SET(TARGET_H
#     axial.hpp
#     bitboard.hpp
#     board.hpp
#     computer.hpp
#     game.hpp
# )

# # Local source files here
# SET(TARGET_SRC
#     axial.cpp
#     bitboard.cpp
#     board.cpp 
#     computer.cpp 
#     game.cpp 
#     main_test.cpp 
# )

file(GLOB srcs
        "${PROJECT_SOURCE_DIR}/game/*.cpp"
)

add_executable(board 
    ${srcs}
)

