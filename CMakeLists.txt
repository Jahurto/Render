cmake_minimum_required(VERSION 3.20)
project(Raytracing_Nextweek)

set(CMAKE_CXX_STANDARD 14)
include_directories(include)
find_package (glog 0.6.0 REQUIRED)
file(GLOB ALLSOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(Raytracing_Nextweek main.cpp ${ALLSOURCES})
target_link_libraries (Raytracing_Nextweek glog::glog)
