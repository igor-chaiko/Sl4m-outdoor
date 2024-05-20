cmake_minimum_required(VERSION 3.26)
project(Sl4m_outdoor)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(Sl4m_outdoor sources/main.cpp
        sources/triangulation.cpp
        headers/triangulation.h
        headers/mapPoint.h
        sources/mapPoint.cpp
        sources/Map.cpp
        headers/Map.h
        sources/processVideo.cpp
        headers/processVideo.h
        sources/mapRebuilding.cpp
        headers/mapRebuilding.h
        headers/loopChecker.h
        sources/loopChecker.cpp
        sources/TrafficSigns.cpp
        headers/TrafficSigns.h
)
target_link_libraries(Sl4m_outdoor ${OpenCV_LIBS})