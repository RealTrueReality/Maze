cmake_minimum_required(VERSION 3.23)
project(Maze)

set(EasyX_DIR C:/Users/d/Downloads/easyx4mingw) # 指定下载的easyx4mingw位置
include_directories(${EasyX_DIR}/include)
link_directories(${EasyX_DIR}/lib64) # 如果是32位系统，则使用lib32

link_libraries(libEasyX.a libgdi32.a libole32.a) # 链接EasyX库


add_executable(Maze main.cpp)

