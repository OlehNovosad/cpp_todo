cmake_minimum_required(VERSION 3.10.0)
project(cpp_todo VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Automatically include all implementation files under src/.
file(GLOB APP_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(cpp_todo main.cpp ${APP_SOURCES})

target_include_directories(cpp_todo PRIVATE "${CMAKE_SOURCE_DIR}/inc" "${CMAKE_SOURCE_DIR}/third_party")
find_package(SQLite3 REQUIRED)
target_link_libraries(cpp_todo PRIVATE SQLite::SQLite3)
