#include "debugger.h"
#include "register.h"
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/ptrace.h>
#include <sys/wait.h>


std::vector<std::string> split (const std::string &s, char delimeter) {
    std::vector<std::string> out{};
    std::istringstream ss {s};
    std::string item;

    // getline默认以 '\n' 作为分割副，此处将分割符改为自定义的 delimeter 
    while (std::getline(ss, item, delimeter)) {
        out.push_back(item);
    }

    return out;
}

bool is_prefix(const std::string& s, const std::string& of) {
    if (s.size() > of.size()) {
        return false;
    }

    // 在指定的范围内比较两个 iterator 是否想等
    return std::equal(s.begin(), s.end(), of.begin());
}



void debugger::run() {
    int wait_status;
    auto options = 0;
    waitpid(m_pid, &wait_status, options);
    // Trace or breakpoint trap, child process stop

    // if (WIFSTOPPED(wait_status) == true) {
    //     std::cout << "Child Process Stopped\n";
    // }

    // 在循环中处理用户的命令
    char* line = nullptr;
    while ((line = linenoise("minidbg> ")) != nullptr) {
        handle_command(line);       // 处理命令行
        linenoiseHistoryAdd(line);  // 添加到历史记录中
        linenoiseFree(line);        // 释放内存
    }

}


void debugger::handle_command(const std::string& line) {
    auto args = split(line, ' ');
    auto command = args[0];

    if (is_prefix(command, "c") || is_prefix(command, "continue")) {

        continue_execution();

    // 处理与断点有关的命令
    } else if (is_prefix(command, "b") || is_prefix(command, "break")) {
        // removed the first two characters of the string, we assume the user has written 0xADDRESS
        std::string addr{args[1], 2};
        set_breakpoint_at_address(std::stol(addr, 0, 16)); // x86系统地址长度为8字节
    
    // 处理与寄存器有关的命令
    } else if (is_prefix(command, "reg") || is_prefix(command, "register")) {
        if (is_prefix(args[1], "dump")) {
            dump_register();
    
        } else if (is_prefix(args[1], "read")) {
            // "register read rax" or "reg read rax" 
            std::cout << std::hex
                      << "0x"
                      << std::hex
                      << get_register_value(m_pid, get_register_from_name(args[2])) << std::endl;
        
        } else if (is_prefix(args[1], "write")) {
            // "register write rax 0x22" or "reg write rax 0x22"
            std::string val {args[3], 2};
            set_register_value(m_pid, get_register_from_name(args[2]), std::stol(val, 0, 16));
        }

    // 处理与内存有关的命令
    } else if (is_prefix(command, "memory")) {
        // "memory read 0xADDRESS" or "memory write 0xADDRESS 0xVAL"
        std::string addr {args[2], 2};
        if (is_prefix(args[1], "read")) {
            std::cout << std::hex << read_memory(std::stol(addr, 0, 16)) << std::endl;
        } else if (is_prefix(args[1], "write")) {
            std::string val {args[3], 2};
            write_memory(std::stol(addr, 0, 16), std::stol(val, 0, 16));
        }
    }
    else {
        std::cerr << "Unknown command\n";
    }
}

void debugger::continue_execution() {
    step_over_breakpoint();
    ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);
    wait_for_signal();
}

void debugger::set_breakpoint_at_address(std::intptr_t addr) {
    std::cout << "Set breakpoint at address 0x" << std::hex << addr << std::endl;
    breakpoint bp (m_pid, addr);
    bp.bp_enable();
    m_breakpoints.insert(std::make_pair(addr, bp));
};


void debugger::dump_register() {
    for (const auto& rd : g_register_descriptors) {
        std::cout << std::setw(10) << std::setfill(' ')
                  << rd.reg_name 
                  << " 0x"
                  << std::setfill('0') << std::setw(16) << std::hex
                  << get_register_value(m_pid, rd.reg_index) << std::endl; 
    }
}



uint64_t debugger::read_memory(std::intptr_t addr) {
   return ptrace(PTRACE_PEEKDATA, m_pid, addr, nullptr);
}


void debugger::write_memory(std::intptr_t addr, uint64_t value) {
    ptrace(PTRACE_POKEDATA, m_pid, addr, value);
}




uint64_t debugger::get_pc() {
    return get_register_value(m_pid, reg_x86_64::rip);
}

void debugger::set_pc(std::intptr_t pc) {
    set_register_value(m_pid, reg_x86_64::rip, pc);
}



void debugger::step_over_breakpoint() {
    auto possiable_breakpoint_location = get_pc() - 1;

    if (m_breakpoints.count(possiable_breakpoint_location)) {
        // auto& bp = m_breakpoints[possiable_breakpoint_location];
        auto& bp = m_breakpoints.at(possiable_breakpoint_location);

        if (bp.is_enabled()) {
            auto previous_instruction_address = possiable_breakpoint_location;
            set_pc(previous_instruction_address);

            bp.bp_disable();
            ptrace(PTRACE_SINGLESTEP, m_pid, nullptr, nullptr);
            wait_for_signal();
            bp.bp_enable();
        }
    }
}

void debugger::wait_for_signal() {
    int wait_status;
    int options = 0;
    waitpid(m_pid, &wait_status, options);
}







