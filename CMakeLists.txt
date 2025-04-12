cmake_minimum_required(VERSION 3.10)
project(leetcode)
add_compile_options(/utf-8)


# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCE src/*.cpp)

# 添加源文件
add_executable(${PROJECT_NAME} ${SOURCE})
target_include_directories(${PROJECT_NAME} PUBLIC .)


