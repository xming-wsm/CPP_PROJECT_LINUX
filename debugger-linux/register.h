#ifndef _REGISTER_H
#define _REGISTER_H


#include <cstddef>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <array>

#ifdef __x86_64__

// x86寄存器数量
constexpr std::size_t n_registers = 27;

// 定义寄存器的枚举类，其顺序与[sys/user.h]中的[user_regs_struct]顺序一致
enum class reg_x86_64 {
    r15 = 0,    r14,        r13,
    r12,        rbp,        rbx,
    r11,        r10,        r9, 
    r8,         rax,        rcx,
    rdx,        rsi,        rdi,
    orig_rax,   rip,        cs,
    eflags,     rsp,        ss,
    fs_base,    gs_base,    ds,
    es,         fs,         gs,
};

struct reg_descriptor {
    reg_x86_64 reg_index;
    int reg_dwarf_number;
    std::string reg_name;
};

// 长度为[n_registers]的结构体[reg_descriptor]的数组
extern const std::array<reg_descriptor, n_registers> g_register_descriptors;


// Get the value of register [reg] of process [pid]
uint64_t get_register_value(pid_t pid, reg_x86_64 reg);

// Set [value] in register [reg] of process [pid]
void set_register_value(pid_t pid, reg_x86_64 reg, uint64_t value);

// // dwarf不太了解，这个函数就照抄了
uint64_t get_register_value_from_dwarf_register(pid_t pid, uint64_t regnum);


// Get the name of specific register
std::string get_register_name(reg_x86_64 reg);

// Get the register from specific name
reg_x86_64 get_register_from_name(const std::string& name);


#endif /* __x86_64__ */





























#endif /* _REGISTER_H */
