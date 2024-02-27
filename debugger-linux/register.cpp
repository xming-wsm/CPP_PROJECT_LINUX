#include "register.h"
#include <cstdint>
#include <iostream>
#include <iterator>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <algorithm>




const std::array<reg_descriptor, n_registers> g_register_descriptors {{
    { reg_x86_64::r15, 15, "r15" },
    { reg_x86_64::r14, 14, "r14" },
    { reg_x86_64::r13, 13, "r13" },
    { reg_x86_64::r12, 12, "r12" },
    { reg_x86_64::rbp, 6, "rbp" },
    { reg_x86_64::rbx, 3, "rbx" },
    { reg_x86_64::r11, 11, "r11" },
    { reg_x86_64::r10, 10, "r10" },
    { reg_x86_64::r9, 9, "r9" },
    { reg_x86_64::r8, 8, "r8" },
    { reg_x86_64::rax, 0, "rax" },
    { reg_x86_64::rcx, 2, "rcx" },
    { reg_x86_64::rdx, 1, "rdx" },
    { reg_x86_64::rsi, 4, "rsi" },
    { reg_x86_64::rdi, 5, "rdi" },
    { reg_x86_64::orig_rax, -1, "orig_rax" },
    { reg_x86_64::rip, -1, "rip" },
    { reg_x86_64::cs, 51, "cs" },
    { reg_x86_64::eflags, 49, "eflags" },
    { reg_x86_64::rsp, 7, "rsp" },
    { reg_x86_64::ss, 52, "ss" },
    { reg_x86_64::fs_base, 58, "fs_base" },
    { reg_x86_64::gs_base, 59, "gs_base" },
    { reg_x86_64::ds, 53, "ds" },
    { reg_x86_64::es, 50, "es" },
    { reg_x86_64::fs, 54, "fs" },
    { reg_x86_64::gs, 55, "gs" },
}};
 


// Get the value of register [r] of process [pid]
uint64_t get_register_value(pid_t pid, reg_x86_64 reg) {
    // 创建结构体[user_regs_struct]，然后使用ptrace函数读取进程[pid]中所有寄存器的值
    user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

    // 以[regs]作为首地址，偏移量为[reg_index]读取寄存器数据
    return *(reinterpret_cast<uint64_t*>(&regs) + (uint64_t)reg);
};



// Set [value] in register [reg] of process [pid]
void set_register_value(pid_t pid, reg_x86_64 reg, uint64_t value) {
    // 在get_register_value函数的基础上，读取reg后，根据索引修改指定寄存器的值
    // 再用ptrace函数重新写入到寄存器中
    user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

    *(reinterpret_cast<uint64_t*>(&regs) + (uint64_t)reg) = value;

    ptrace(PTRACE_SETREGS, pid, nullptr, &regs);
}


// // DWARF提供了许多有用的调试信息 https://blog.csdn.net/chenyijun/article/details/85284867 
// uint64_t get_register_value_from_dwarf_register (pid_t pid, unsigned regnum) {
//     auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
//                            [regnum](auto&& rd) { return rd.reg_dwarf_number == regnum; });
//     if (it == end(g_register_descriptors)) {
//         throw std::out_of_range{"Unknown dwarf register"};
//     }
//
//     return get_register_value(pid, it->reg_index);
// }
//


// Get the name of specific register
std::string get_register_name(reg_x86_64 reg) {
    auto it = std::find_if(std::begin(g_register_descriptors), std::end(g_register_descriptors),
                           [reg](auto && rd) {return rd.reg_index == reg;});
    if (it == std::end(g_register_descriptors)) {
        std::cout << "Wrong Register index\n";
    }
    return it->reg_name;
}


// Get the register from specific name
reg_x86_64 get_register_from_name(const std::string& name) {
    auto it = std::find_if(std::begin(g_register_descriptors), std::end(g_register_descriptors),
                           [name](auto && rd) {return rd.reg_name == name;});
    if (it == std::end(g_register_descriptors)) {
        std::cout << "Wrong Register name!\n"; 
    }
    return it->reg_index;
}




















