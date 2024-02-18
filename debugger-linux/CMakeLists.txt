cmake_minimum_required(VERSION 3.10)
project(minidebug)

# 添加可执行文件并指定源文件
add_executable( minidebug main.cpp linenoise.c linenoise.h 
                debugger.h      debugger.cpp
                breakpoint.h    breakpoint.cpp
                register.h      register.cpp)
