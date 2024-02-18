#ifndef _DEBUGGER_H
#define _DEBUGGER_H

#include <iterator>
#include <pthread.h>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <cstdint>


#include "linenoise.h"
#include "breakpoint.h"

#include <sys/user.h>


// 根据分割符[delimeter]分割字符串[s]
std::vector<std::string> split (const std::string &s, char delimeter);
// 判断字符串[s]与字符串[of]是否相等
bool is_prefix(const std::string& s, const std::string& of);


class debugger {

public:
    debugger (std::string prog_name, pid_t pid) 
        : m_prog_name{std::move(prog_name)}, m_pid{pid} {};
    // 

    // 启动debug
    void run();
    // 处理用户输入命令[line]
    void handle_command(const std::string& line);
    // 继续执行
    void continue_execution();
    // 根据用户输入地址[addr]设置断点
    void set_breakpoint_at_address(std::intptr_t addr);
    // 打印寄存器信息
    void dump_register();
    // 读取内存数据
    uint64_t read_memory(std::intptr_t addr); 
    // 写内存数据
    void write_memory(std::intptr_t addr, uint64_t value);
    // 读程序计数器PC
    uint64_t get_pc();
    // 写程序计数器PC
    void set_pc(std::intptr_t pc);
    // step over a breakpoint - 执行光标所对应的行的代码，不会进入到子函数中。
    void step_over_breakpoint();
    // 调用waitpid函数，等待信号
    void wait_for_signal();

private:
    std::string m_prog_name;    // 可执行二进制文件的名字
    pid_t m_pid;    

    // 键值哈系表，存储断电与地址的映射关系
    std::unordered_map<std::intptr_t, breakpoint> m_breakpoints;   
};


#endif /* _DEBUGGER_H */
