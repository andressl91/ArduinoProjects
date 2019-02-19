cmake_minimum_required(VERSION 3.13)

project(socket)

include_directories(include)

set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -Lboost -lpthread")

file(GLOB SOURCES "src/*.cpp")

add_executable(server ${SOURCES})



