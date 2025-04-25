cmake_minimum_required(VERSION 3.16)

project(infinite_sense_core VERSION 0.1.0)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake CACHE INTERNAL "" FORCE)
find_package(ZeroMQ REQUIRED)

set(PROJECT_INSTALL_DIR ${PROJECT_NAME})

add_compile_options(-fPIC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 确保强制使用指定的标准
set(CMAKE_CXX_EXTENSIONS OFF)       # 禁用编译器特定的扩展（推荐）

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif ()
add_subdirectory(third_party/serial)
add_subdirectory(third_party/udp)
add_library(${PROJECT_NAME} SHARED
    src/infinite_sense.cpp
    src/trigger_manager.cpp
    src/serial_manager.cpp
    src/cam_manager.cpp
    src/image.cpp
    src/net_manager.cpp
    src/messenger.cpp
    src/ptp.cpp
    include/ptp.h
    src/sensor.h
    include/messenger.h
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
    ${ZeroMQ_INCLUDE_DIRS}
)

# 链接库
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${ZeroMQ_LIBRARIES}
    MvCameraControl
    serial
    udp
)

# 包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
    include
    third_party/mvcam/include
)
# 设置目标的 RPATH
set_target_properties(${PROJECT_NAME} PROPERTIES
    INSTALL_RPATH "$ORIGIN"
)

# 如果是构建的动态库，也可以设置构建时的 RPATH（即在开发过程中）
set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_RPATH "$ORIGIN"
)

# 安装库文件和头文件到平台特定目录
# 安装目标库
install(TARGETS ${PROJECT_NAME} serial udp
    DESTINATION ${PROJECT_INSTALL_DIR}/${PLATFORM_DIR}/lib)

# 安装头文件目录
install(DIRECTORY include/
    DESTINATION ${PROJECT_INSTALL_DIR}/${PLATFORM_DIR}/include)

# 安装 MVCAM 库文件
if (MVCAM_LIB_DIR)
  install(DIRECTORY ${MVCAM_LIB_DIR}/
      DESTINATION ${PROJECT_INSTALL_DIR}/${PLATFORM_DIR}/lib)
endif ()