cmake_minimum_required(VERSION 3.10)

# 项目信息
project(my_cpp_project VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 设置输出目录到 build/ 目录下
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 遍历src目录下的所有子目录并添加
file(GLOB SUBDIRS RELATIVE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/*)

foreach(subdir ${SUBDIRS})
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/src/${subdir})
        add_subdirectory(src/${subdir})
    endif()
endforeach()

# 外部库目录（可选）
# add_subdirectory(lib)
