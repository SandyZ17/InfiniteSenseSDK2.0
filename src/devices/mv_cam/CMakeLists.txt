cmake_minimum_required(VERSION 3.16)

project(mv_camera VERSION 0.1.0)


add_library(${PROJECT_NAME} SHARED
    src/image.cpp
    src/mv_camera.cpp
)


# 检测平台并设置相关变量
if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
  set(PLATFORM_DIR "arm")
  set(MVCAM_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/mvcam/lib/aarch64)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "amd64")
  set(PLATFORM_DIR "x86")
  set(MVCAM_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/mvcam/lib/x86/64)
else ()
  message(STATUS "Unknown Architecture")
  set(PLATFORM_DIR "Unknown")
endif ()
message(STATUS "Find MVCAM_LIB_DIR : ${MVCAM_LIB_DIR}")
# 设置目标链接目录
target_link_directories(${PROJECT_NAME} PUBLIC
    BEFORE ${MVCAM_LIB_DIR}
    ${CMAKE_INSTALL_RPATH}
)


# 包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
    include
    third_party/mvcam/include
    ../../infinite_sense_core/include
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    MvCameraControl
)

