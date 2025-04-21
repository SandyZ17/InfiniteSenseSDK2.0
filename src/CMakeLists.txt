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

add_executable(${PROJECT_NAME}_node
  main.cpp
)

target_link_libraries(${PROJECT_NAME}_node PRIVATE pthread infinite_sense_core)


######################### 一些参考的同步Demo #################################

# 1. VIO 多个海康/大恒等工业相机同步和IMU同步

add_subdirectory(devices/mv_cam)

add_executable(${PROJECT_NAME}_mv_cam_node
  examples/mv_camera.cpp
)
target_link_libraries(${PROJECT_NAME}_mv_cam_node PRIVATE pthread infinite_sense_core mv_camera )

# 