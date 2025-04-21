cmake_minimum_required(VERSION 3.12)
project(ispy)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Python COMPONENTS Interpreter Development REQUIRED)
if(Python_FOUND)
    message(STATUS "Python found: ${Python_EXECUTABLE}")
else()
    message(FATAL_ERROR "Python not found")
endif()

find_package(pybind11 REQUIRED)
#add_subdirectory(python)


add_library(my_module MODULE my_module.cpp
        my_module.cpp
        ../../ardunio/new_core_rtk/src/utlis/utlis.h)
target_include_directories(my_module PRIVATE ${Python_INCLUDE_DIRS} ${pybind11_INCLUDE_DIRS})
target_link_libraries(my_module PRIVATE ${Python_LIBRARIES} ${pybind11_LIBRARIES})
