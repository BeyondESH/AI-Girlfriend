cmake_minimum_required(VERSION 3.31)
project(AI_Girlfriend)

set(CMAKE_CXX_STANDARD 20)

file(GLOB HEADER_FILES "include/*.h" "include/*.hpp")
file(GLOB SOURCE_FILES "src/*.cpp")
set(ALL_FILES ${HEADER_FILES} ${SOURCE_FILES})

## 添加头文件目录
include_directories(include)

# 添加库文件搜索路径
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/cmake")

# 查找PortAudio库
find_package(portaudio QUIET)

add_executable(AI_Girlfriend ${ALL_FILES})

# 链接库文件
target_link_directories(AI_Girlfriend PRIVATE lib)

# 链接vosk库
target_link_libraries(AI_Girlfriend vosk)

# 链接PortAudio库
if(portaudio_FOUND)
    target_link_libraries(AI_Girlfriend portaudio)
    message(STATUS "Found PortAudio via CMake config")
else()
    # 如果find_package失败，直接链接静态库
    target_link_libraries(AI_Girlfriend portaudio_static_x64)
    message(STATUS "Using PortAudio static library directly")
endif()

# Windows系统下需要链接的系统库
if(WIN32)
    target_link_libraries(AI_Girlfriend
        winmm
        ole32
        oleaut32
        uuid
        advapi32
    )
endif()
