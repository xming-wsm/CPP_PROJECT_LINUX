#ifndef _DEBUGGER_H
#define _DEBUGGER_H

#include <bits/types/siginfo_t.h>
#include <cstdio>
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
#include <sys/user.h>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>


#include "linenoise.h"
#include "breakpoint.h"
#include "libelfin/elf/elf++.hh"
#include "libelfin/dwarf/dwarf++.hh"


enum class symbol_type {
    notype,     // No type (e.g., absolute symbol)
    object,     // Data object
    func,       // Funtion entry point
    section,    // Source file associated with the object file
    file,
};


// 对于[.symtab]中的数据，只关心[type], [name], [addr(value)]
struct symbol {
    symbol_type type;
    std::string name;
    std::uintptr_t addr;
};




// 根据分割符[delimeter]分割字符串[s]
std::vector<std::string> split (const std::string &s, char delimeter);
// 判断字符串[s]与字符串[of]是否相等
bool is_prefix(const std::string& s, const std::string& of);
bool is_suffix(const std::string& s, const std::string& of);

// 返回[st]对应的字符串文本
std::string to_string(symbol_type st);
// 将从[libelfin]获得的符号类型与我们定义的enum互相映射
symbol_type to_symbol_type(elf::stt sym);







class debugger {

public:
    // 初始化函数
    debugger (std::string prog_name, pid_t pid) 
        : m_prog_name{std::move(prog_name)}, m_pid{pid} {

        // 根据可执行文件路径实例化[m_elf]与[m_drawf];
        int fd = open(m_prog_name.c_str(), O_RDONLY);
        m_elf = elf::elf{elf::create_mmap_loader(fd)};
        m_dwarf = dwarf::dwarf{dwarf::elf::create_loader(m_elf)};
    }


    // 启动debug
    void run();
    // 处理用户输入命令[line]
    void handle_command(const std::string& line);
    // 继续执行
    void continue_execution();
    // 根据用户输入地址[addr]设置断点
    void set_breakpoint_at_address(std::intptr_t addr);
    // 根据函数名字设置断点
    void set_breakpoint_at_function(const std::string& name);
    // 根据源代码行数设置断点
    void set_breakpoint_at_source_line(const std::string& file, uint64_t line);
    // 移除断点
    void remove_breakpoint_at_address(std::intptr_t addr);
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



    /* Funciton using [dwarf] and [elf] */
    // 根据pc判断目前所在的函数 
    dwarf::die get_function_from_pc(uint64_t pc);
    // 根据pc判断目前地址对应的源代码行数
    dwarf::line_table::iterator get_line_entry_from_pc(uint64_t pc);
    // 初始化加载地址
    void initialise_load_address();
    // 进行加载地址偏置
    uint64_t offset_dwarf_address(uint64_t addr);
    // 去掉加载地址偏偏置
    uint64_t  offset_load_address(uint64_t addr);
    // 封装，得到去偏置后的PC地址
    uint64_t get_current_pc_offset_address();

    // 根据指定行数打印源文件代码文本及其上下文
    void print_source(const std::string& file_name, uint64_t specify_line);
    
    // 获取信号量相关信息
    siginfo_t get_signal_info();
    // 信号量处理函数
    void handle_sigtrap(siginfo_t info);

    // 指令级单步步进
    void single_step_instruction();
    // 带有短的检查的指令级单步步进
    void single_step_instruction_with_breakpoint_check();
    // 跳出
    void step_out();
    // 单步执行(逐语句)
    void step_in();
    // 逐过程
    void step_over();


    // symbol file
    std::vector<symbol> lookup_symbol(const std::string& name);
    // 获取函数信息
    void dwarf_function_information(const std::string& file_name);

    // 打印函数堆栈
    void print_backtrace();

private:
    std::string m_prog_name;    // 可执行二进制文件的名字
    pid_t m_pid;    

    // 键值哈系表，存储断电与地址的映射关系
    std::unordered_map<std::intptr_t, breakpoint> m_breakpoints;   

    // 使用dwarf和elf
    dwarf::dwarf m_dwarf;
    elf::elf m_elf;

    // 可执行文件的加载初始地址
    uint64_t m_load_address;
    
};


#endif /* _DEBUGGER_H */
