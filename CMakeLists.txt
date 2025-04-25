cmake_minimum_required(VERSION 3.16)

project(infinite_sense VERSION 1.0)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake CACHE INTERNAL "" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 确保强制使用指定的标准
set(CMAKE_CXX_EXTENSIONS OFF)       # 禁用编译器特定的扩展（推荐）

add_subdirectory(infinite_sense_core)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3  -Wall")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3  -Wall")
endif()

find_package(ZeroMQ QUIET)
if (ZeroMQ_FOUND)
  message(STATUS "ZMQ found, Compiling ZMQ nodes")
  find_package(ZeroMQ REQUIRED)
  add_executable(${PROJECT_NAME}_zmq_node
          example/ZMQ/zmq_main.cpp
  )
  target_include_directories(${PROJECT_NAME}_zmq_node PRIVATE
      ${ZeroMQ_INCLUDE_DIRS}
  )
  target_link_libraries(${PROJECT_NAME}_zmq_node PRIVATE
      ${ZeroMQ_LIBRARIES}
      infinite_sense_core
  )
endif ()
