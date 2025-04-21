cmake_minimum_required(VERSION 3.16)

project(infinite_sense_core VERSION 0.1.0)

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
    src/serial_manager.cpp
    src/net_manager.cpp
    src/ptp.cpp
)


# 链接库
target_link_libraries(${PROJECT_NAME} PUBLIC
    serial
    udp
)

# 包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
    include
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
