cmake_minimum_required(VERSION 3.16)
# 设置默认构建类型
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
# 项目使用C++20标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(coreutils LANGUAGES CXX)
# FetchContent模块: 允许从外部仓库直接下载依赖项
# FetchContent_Declare指定库的Git仓库位置
# FetchContent_MakeAvailable下载并使库可用于当前项目。
include(FetchContent)
# range-v3: 提供强大范围操作的现代C++库
# cxxopts: 轻量级C++选项解析库
FetchContent_Declare(cxxopts GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git)
FetchContent_MakeAvailable(cxxopts)

add_executable(hexdump hexdump.cpp)
# PRIVATE: 指定的库或目标只用于该目标的编译和链接
# 避免依赖传递
target_link_libraries(hexdump PRIVATE cxxopts)