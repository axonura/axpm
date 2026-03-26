cmake_minimum_required(VERSION 3.12)
project("axpm")

find_package(LibArchive REQUIRED)
find_package(jsoncpp REQUIRED)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_subdirectory(sample)
add_subdirectory(src)