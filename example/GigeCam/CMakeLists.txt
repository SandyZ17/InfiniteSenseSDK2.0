cmake_minimum_required(VERSION 3.16)

project(gige_cam VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3  -Wall")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3  -Wall")
endif ()


# 检测平台并设置相关变量
if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(PLATFORM_DIR "arm")
    set(MVCAM_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mvcam/lib/aarch64)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "amd64")
    set(PLATFORM_DIR "x86")
    set(MVCAM_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mvcam/lib/x86/64)
else ()
    message(STATUS "Unknown Architecture")
    set(PLATFORM_DIR "Unknown")
endif ()
message(STATUS "Find MVCAM_LIB_DIR : ${MVCAM_LIB_DIR}")


add_executable(${PROJECT_NAME}
        main.cpp
        mv_cam.cpp
        mv_cam.h
)
target_link_directories(${PROJECT_NAME} PRIVATE
        ${MVCAM_LIB_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        infinite_sense_core
        MvCameraControl
)

target_include_directories(${PROJECT_NAME} PRIVATE
        mvcam/include
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH "$ORIGIN"
)