cmake_minimum_required(VERSION 3.16)

project(monitor VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3  -Wall")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3  -Wall")
endif ()

add_executable(${PROJECT_NAME}
        main.cpp
        monitor.cpp
        monitor.h
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        infinite_sense_core
)
