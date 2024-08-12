cmake_minimum_required(VERSION 3.26)
project(hillelbot)

set(SRC "${CMAKE_SOURCE_DIR}/src")
include_directories(${SRC})

add_executable(${PROJECT_NAME} ${SRC}/main.cxx)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20
                                                 CXX_STANDARD_REQUIRED ON)