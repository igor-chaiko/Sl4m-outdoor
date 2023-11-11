cmake_minimum_required(VERSION 3.26)
project(Sl4m_outdoor)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(Sl4m_outdoor main.cpp
        triangulation.cpp
        triangulation.h
        mapPoint.h
        mapPoint.cpp
        drawMap.cpp
        drawMap.h)
target_link_libraries(Sl4m_outdoor ${OpenCV_LIBS})