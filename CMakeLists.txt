set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

cmake_minimum_required(VERSION 3.26)
project(hillelbot)

set(SRC "${CMAKE_SOURCE_DIR}/src")
include_directories(${SRC})

find_package(nlohmann_json REQUIRED)
find_package(cpr REQUIRED)

add_executable(${PROJECT_NAME} ${SRC}/main.cxx)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20
                                                 CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr nlohmann_json::nlohmann_json)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})