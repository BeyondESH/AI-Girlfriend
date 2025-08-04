cmake_minimum_required(VERSION 3.31)
project(AI_Girlfriend)

set(CMAKE_CXX_STANDARD 20)

## 添加头文件目录
include_directories(include)

add_executable(AI_Girlfriend src/main.cpp
        include/Singleton.hpp
        src/ThreadPool.cpp
        include/ThreadPool.h)

# 链接库文件
target_link_directories(AI_Girlfriend PRIVATE lib)
target_link_libraries(AI_Girlfriend vosk)