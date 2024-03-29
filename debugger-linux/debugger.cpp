#include "debugger.h"
#include "ptrace_expr_context.h"
#include "register.h"
#include <algorithm>
#include <bits/types/siginfo_t.h>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <libelfin/elf/data.hh>
#include <ostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <vector>

#include "libelfin/dwarf/data.hh"
#include "libelfin/dwarf/dwarf++.hh"


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
    if (!strcmp(s.c_str(), of.c_str())) {
        return true;
    }
    return false;
}



bool is_suffix(const std::string& s, const std::string& of) {
    if (s.size() > of.size()) return false;
    auto diff = of.size() - s.size();
    return std::equal(s.begin(), s.end(), of.begin() + diff);
}



std::string to_string(symbol_type st) {
    switch (st) {
        case symbol_type::notype:   return "notype";
        case symbol_type::object:   return "object";
        case symbol_type::file:     return "file";
        case symbol_type::func:     return "func";
        case symbol_type::section:  return "section";
        default: return "Unknown symbol_type st";
    }
}


symbol_type to_symbol_type(elf::stt sym) {
    switch (sym) {
        case elf::stt::notype:      return symbol_type::notype;
        case elf::stt::object:      return symbol_type::object;
        case elf::stt::file:        return symbol_type::file;
        case elf::stt::section:     return symbol_type::section;
        case elf::stt::func:        return symbol_type::func;
        default: return symbol_type::notype;
    }
}




void debugger::run() {
    wait_for_signal();
    initialise_load_address();
    // std::cout << "loaded address 0x" << std::hex << m_load_address << std::endl;
    // auto func = get_function_from_pc(get_current_pc_offset_address());
    // std::cout << dwarf::at_name(func) << std::endl;


    // Test [set_breakpoint_at_source_line];
    // set_breakpoint_at_source_line("test.c", 17);

    // Test [dwarf_function_information()]


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
        // input command: "b 0xADDRESS" or "break 0xADDRESS"
        // stol(addr, nullptr, 16) 即把16进制的地址转为10进制的long

        if (args[1][0] == '0' && args[1][0] == 'x') {
            // 1. 根据地址设置断点: b 0xADDRESS
            std::string addr{args[1], 2};
            set_breakpoint_at_address(std::stol(addr, nullptr, 16)); // x86系统地址长度为8字节
            std::cout << "set breakpoint at 0x" << std::hex << std::stol(addr, nullptr, 16);
        
        } else if (args[1].find(':') != std::string::npos) {
            // 2. 根据源文件代码行数设置地址: b <file>:<line> 
            auto file_and_line = split(args[1], ':');
            set_breakpoint_at_source_line(file_and_line[0], std::stol(file_and_line[1]));
            std::cout << "set breakpoint at [" << file_and_line[0] << ":" << file_and_line[1]
                      << "]:\t";
            print_source(file_and_line[0], std::stol(file_and_line[1]));
        } else {
            // funcion相关函数会出现问题
            set_breakpoint_at_function(args[1]);
        }


    // 处理与寄存器有关的命令
    } else if (is_prefix(command, "reg") || is_prefix(command, "register")) {
        if (is_prefix(args[1], "dump")) {
            dump_register();
    
        } else if (is_prefix(args[1], "read")) {
            // "register read rax" or "reg read rax" 
            std::cout << "0x"
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


    // 指令级单步步进
    }  else if (is_prefix(command, "stepi")) {
        single_step_instruction_with_breakpoint_check();
        auto line_entry = get_line_entry_from_pc(get_pc());
        print_source(line_entry->file->path, line_entry->line);

    // 逐过程
    } else if (is_prefix(command, "next") || is_prefix(command, "n")) {
        step_over();


    // 逐语句
    } else if (is_prefix(command, "step") || is_prefix(command, "s")) {
        // std::cout << "step in" << std::endl;
        step_in();

    // 跳出
    } else if (is_prefix(command, "finish")) {
        step_out();

    } else if (is_prefix(command, "symbol")) {
        auto syms = lookup_symbol(args[1]);
        for (auto &&s : syms) {
            std::cout << s.name << ' ' << to_string(s.type) << "0x" << std::hex << s.addr << std::endl;
        }
    } else if (is_prefix(command, "function")) {
        dwarf_function_information("./build/function_information.txt");
    

    } else if (is_prefix(command, "backtrace")) {
        print_backtrace();
    

    } else if (is_prefix(command, "variables")) {
        read_variables();
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
    // std::cout << "Set breakpoint at address 0x" << std::hex << addr << std::endl;
    breakpoint bp (m_pid, addr);
    bp.bp_enable();
    m_breakpoints.insert(std::make_pair(addr, bp));
};


/*
 * @brief: 通过[dwarf]获取函数信息与其起始地址，随后调用[set_breakpoint_at_address]
 *         设置断点
 */
void debugger::set_breakpoint_at_function(const std::string& name) {
    for (const auto& cu : m_dwarf.compilation_units()) {
        for (const auto& die : cu.root()) {
            if (die.has(dwarf::DW_AT::name) && dwarf::at_name(die) == name) {
                // low_pc 是函数的起始地址（start address of the funcion）
                auto low_pc = dwarf::at_low_pc(die);
                auto entry = get_line_entry_from_pc(low_pc);
                entry ++; // skpi prologue
                set_breakpoint_at_address(offset_dwarf_address(entry->address));
            }
        }

    }
}

/*
 * @brief: 根据[.debug_line]中的信息，在对应的地址出打上断点。
 */
void debugger::set_breakpoint_at_source_line(const std::string& file, uint64_t line) {
    // 遍历[DWARF]中的编译单元
    for (const auto& cu : m_dwarf.compilation_units()) {
        // 编译单元含中有名字信息，调用[is_suffix]函数，比较CU中的文件名与[file]
        if (is_suffix(file, dwarf::at_name(cu.root()))) {
            // 获取[line_table]信息，找到对应的行数
            const auto& it = cu.get_line_table();
            for (const auto& entry : it) {
                if (entry.is_stmt && entry.line == line) {
                    // 若行数与[line]相匹配，则在对应地址设置断点
                    set_breakpoint_at_address(offset_dwarf_address(entry.address));
                    return;
                }
            }
        }
        
    }

}





void debugger::remove_breakpoint_at_address(std::intptr_t addr) {
    if (m_breakpoints.at(addr).is_enabled()) {
        m_breakpoints.at(addr).bp_disable();
    }
    // 删除断点后，擦出数据中的数据
    m_breakpoints.erase(addr);
}



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
    uint64_t possiable_breakpoint_location = get_pc();
    // std::cout << "possiable_breakpoint_location - 0x" 
    //           << std::hex << possiable_breakpoint_location << std::endl;
    if (m_breakpoints.count(possiable_breakpoint_location)) {
        auto& bp = m_breakpoints.at(possiable_breakpoint_location);
        // std::cout << "Step over breakpoint at 0x" << std::hex << bp.get_address() << std::endl;

        if (bp.is_enabled()) {
            bp.bp_disable();
            ptrace(PTRACE_SINGLESTEP, m_pid, nullptr, nullptr);
            wait_for_signal();
            // std::cout << "Parent process recieve the signal." << std::endl;
            bp.bp_enable();
        }
    }
}

void debugger::wait_for_signal() {
    int wait_status;
    int options = 0;
    waitpid(m_pid, &wait_status, options);

    auto siginfo = get_signal_info();

    switch (siginfo.si_signo) {
        case SIGTRAP:
            handle_sigtrap(siginfo);
            break;

        case SIGSEGV:
            std::cout << "Yay, segfalut. Reason: " << siginfo.si_code << std::endl;
            break;

        default:
            std::cout << "Got Signal" << strsignal(siginfo.si_signo) << std::endl;
    }

}



/**
 * 1. 首先，通过对 m_dwarf 的迭代器访问 compilation_units，遍历所有的编译单元（Compilation Units）。
 * 2. 对于每个编译单元 cu，检查它的根 DIE（Debugging Information Entry）的地址范围是否包含给定的指令地址 pc。
 * 3. 如果包含，就遍历该编译单元中的所有 DIE，找到 tag 为 subprogram 的 DIE。
 * 4. 对于每个 subprogram DIE，再次检查其地址范围是否包含给定的指令地址 pc。
 * 5. 如果包含，就返回该 subprogram DIE，表示找到了包含该指令地址的函数信息。
 * 6. 如果没有找到，则继续遍历下一个编译单元，直到遍历完所有编译单元或者找到匹配的函数信息
 * 7. 若遍历完所有的单元都没有找到，则抛出错误
*/
dwarf::die debugger::get_function_from_pc(uint64_t pc) {
    const std::vector<dwarf::compilation_unit> &uints = m_dwarf.compilation_units();
    for (const auto& cu : uints) {
        // Compilation Unit包含多个IDEs，如果pc在某个CU中，则需要遍历该Cu的IDEs，判断其tag
        if (dwarf::die_pc_range(cu.root()).contains(pc)) {
            for (const auto& die : cu.root()) {
                // std::cout << die.tag << std::endl;
                if (die.tag == dwarf::DW_TAG::subprogram) {
                    // std::cout << dwarf::at_name(die) << std::endl;

                    // function的IDE的tag一定是subprogram，但是
                    // 库函数是不包含 DW_AT_Low_pc的，需要添加
                    // 一个条件进行判断。
                    // main函数不需要排除
                    if (die.has(dwarf::DW_AT::prototyped) && dwarf::at_name(die)!= "main") {
                        continue;
                    }
                    if (dwarf::die_pc_range(die).contains(pc)) {
                        return die;
                    }
                }
            }
        }
    }
    throw std::out_of_range{"Can't find function"};
}

/**
 * @biref: 根据pc地址找到[line_table]中对应的行数
 * 
 * 1. 通过对 m_dwarf 的迭代器访问 compilation units，遍历所有编译单元
 * 2. 检查 Cu 的 die 的地址是否包含给定的指令地址pc，如果包含，就获取该编译单元的 line talbe
 * 3. 通过对 line table 的迭代器，调用其成员函数 find_address，找到地址pc对应的行数
 * 4. 如果 find_address 的返回值为 line table 的最后一个，说明没有找到，超出范围
 * 5. 否则，将 fine_address 的返回值作为 get_line_entry_from_pc 的返回值并进行返回
 */
dwarf::line_table::iterator debugger::get_line_entry_from_pc(uint64_t pc) {
    const std::vector<dwarf::compilation_unit> &uints = m_dwarf.compilation_units();
    for (const auto& cu : uints) {
        // cu 有问题，掉用 cu 的成员函数会报错
        if (die_pc_range(cu.root()).contains(pc)) {
            auto &It = cu.get_line_table();
            auto it = It.find_address(pc);
            if (it == It.end()) {
                throw std::out_of_range{"Can't find line entry"};
            } else {
                return it;
            }
        }
    }
    throw std::out_of_range{"Can't find line entry"};
}


/**
 * 二进制文件是有其加载地址(loaded address)，而 dwarf 中所给出的地址都是基于加载地址
 * 为了在正确的地址设置断点，我们需要在 dwarf 的基础上偏执一个 loaded address
 * loaded address 可由 "proc/<pid>/maps" 的第一行获得
 */
void debugger::initialise_load_address() {
    if (m_elf.get_hdr().type == elf::et::dyn) {
        std::ifstream map("/proc/" + std::to_string(m_pid) + "/maps");
        // 读取file第一行中的地址
        std::string addr;
        std::getline(map, addr, '-');

        m_load_address = std::stol(addr, 0, 16);
    }
}



uint64_t debugger::offset_load_address(uint64_t addr) {
    if (addr >= m_load_address) {
        return addr - m_load_address;
    } else {
        std::cerr << "Addr should greater or equal to m_load_address!";
    }
    return -1;
}


uint64_t debugger::offset_dwarf_address(uint64_t addr) {
    return addr + m_load_address;
}



uint64_t debugger::get_current_pc_offset_address() {
    return offset_load_address(get_pc());
}







/**
 * 在指定文件[file_name]中，根据指定行数[line]及其周围上下文行数[n_lines_context]
 * 打印代码内容，并在指定行号处添加标记，显示当前行
 */
void debugger::print_source(const std::string& file_name, uint64_t specify_line) {
    std::ifstream file {file_name};
    uint64_t current_line = 0;
    std::string line;

    while (current_line < specify_line && std::getline(file, line)) {
        current_line ++;
    }

    if (current_line == specify_line) {
        printf("%ld %s\n", current_line, line.c_str());
    } else {
        throw std::out_of_range{"print_source(): specify_line is out of range!"};
    }
}


/**
 * 通过结构体[siginfo_t]与函数[ptrace]获取信号量的具体信息
 */ 
siginfo_t debugger::get_signal_info() {
    siginfo_t info;
    ptrace(PTRACE_GETSIGINFO, m_pid, nullptr, &info);
    return info;
}


/**
 * @breif: 根据不同的[info.si_signo]执行不同的任务
 */
void debugger::handle_sigtrap(siginfo_t info) {
    switch (info.si_code) {

        // 当运行到断点处时，会接受到如下信号了
        case SI_KERNEL:
        case TRAP_BRKPT: {
            // put the pc back where it should be
            set_pc(get_pc() - 1);

            // std::cout << "Hit breakpoint at address 0x" 
            //           << std::hex << std::setfill('0') << get_pc() << std::endl;
            //

            // [get_pc]的返回地址是栈地址，而[dwarf]里的地址信息都是以
            // loaded address的相对地址。因此，从[get_pc]得到的地址需要 
            // 减去loadded address.
            // auto offset_pc = offset_load_address(get_pc()); 
            // auto line_entry = get_line_entry_from_pc(offset_pc);
            
            // std::cout << "123213" << std::endl;
            auto line_entry = get_line_entry_from_pc(get_current_pc_offset_address());
            
            print_source(line_entry->file->path, line_entry->line);
            return;

        }

        // This will be set if the signal was sent by single stepping
        case TRAP_TRACE:
            return;
        default:
            std::cout << "Unknown SIGTRAP code " << info.si_code << std::endl;
            return;
    }
}



/*
 * @brief: 在不检查当前位置是否有断点的情况下，让子进程单步执行指令。
 * @note: 本函数与single_step_instruction_with_breakpoint_check函数都是
 *        指令级操作（instruction)，更加的底层。
 *        而step_over, step_in, step_out函数都是以代码段为操作，相对来说
 *        更上层一些。
 **/
void debugger::single_step_instruction() {
    // PTRACE_SINGLESTEP: single step the process
    // 因此，当被监视的进程执行完[single step]后，就会向
    // 父进程发送信号量。所以父进程要调用[wait_for_signal];
    ptrace(PTRACE_SINGLESTEP, m_pid, nullptr, nullptr);
    wait_for_signal();
}



/*
 * 在检查当前位置是否为断点，随后让子进程单步执行
 **/
void debugger::single_step_instruction_with_breakpoint_check() {
    if(m_breakpoints.count(get_pc())) { 
        // 运行到断点出，如要继续单步执行需要step over断点
        step_over_breakpoint();
    } else {
        single_step_instruction();
    }
}




void debugger::step_out() {
    auto frame_point = get_register_value(m_pid, reg_x86_64::rbp);
    // 使用[read_memory]读取函数返回地址
    auto return_address = read_memory(frame_point + 8);


    // 有两种情况需要讨论
    // 1. return_address 所在的地址本身就有一个断点，则不需要打断点，直接
    //    [continue_execution]函数即可
    // 2. return_address 所在的地址本身没有断点。
    //    （1）打断点   set_break_at_address()
    //    （2）执行     continue_exeuction()
    //    （3）消除断点 remove_breakpoint_at_address()
    bool should_remove_breakpoint = false;
    if (!m_breakpoints.count(return_address)) {
        set_breakpoint_at_address(return_address);
        should_remove_breakpoint = true;
    }

    continue_execution();

    if (should_remove_breakpoint) {
        remove_breakpoint_at_address(return_address);
    }
}



/** 
 * @brief: 单步步进(逐语句)
 *         一直调用[single_step_instruction]，直到代码运行到新的一行
 * 
 **/
void debugger::step_in() {
    auto line = get_line_entry_from_pc(get_current_pc_offset_address())->line;

    // 如果当前行数等于最开始的行数，执行指令级单步步进
    while(get_line_entry_from_pc(get_current_pc_offset_address())->line == line) {
        single_step_instruction_with_breakpoint_check();
    }

    // 获取去偏置后的pc地址，在[DWARF - line_table]中查找对应的代码行
    // 并打印对应的文本信息
    auto line_entry = get_line_entry_from_pc(get_current_pc_offset_address());
    print_source(line_entry->file->path, line_entry->line);
}




/**
 * @brief: 逐过程调试。遇到子函数时会执行子函数，但是不会进入子函数内部
 *         而是直接返回。
 **/
void debugger::step_over() {
    // Get the low pc and high pc values for the given function DIE.
    auto func = get_function_from_pc(get_current_pc_offset_address()); // 当前所在函数
    auto func_entry = dwarf::at_low_pc(func);                          // 当前所在函数起始地址
    auto func_end = dwarf::at_high_pc(func);                           // 当前所在函数结束地址

    auto line = get_line_entry_from_pc(func_entry);       // 函数入口对应的行数
    auto start_line = get_line_entry_from_pc(func_entry); // 当前pc对应的行数
    

    // 创建一个向量，用于保存添加的地址。用于最后的删除。
    std::vector<std::intptr_t> to_delete{};
    
    // 为了设置断点，从[dwarf]得到的地址都要先进行偏置
    // 从[line]开始设置断点，跳过[start_line]，直到[func_end];

    while (line->address < func_end) {
        // 对地址进行偏置
        auto load_address = offset_dwarf_address(line->address);
        if (line->address != start_line->address && !m_breakpoints.count(load_address)) {
            // line不等于[start_line]且本身不为断点
            set_breakpoint_at_address(load_address);
            to_delete.push_back(load_address);
        }
        line ++;
    }
    // Setting a breakpiont on the return address of the funcion, just like in [step_out]
    auto frame_pointer = get_register_value(m_pid, reg_x86_64::rbp);
    auto return_address = read_memory(frame_pointer + 8);
    if (!m_breakpoints.count(return_address)) {
        set_breakpoint_at_address(return_address);
        to_delete.push_back(return_address);
    }


    // 继续执行程序，直到某个断点被命中，则消除掉所有添加的断点
    continue_execution();

    for (auto addr : to_delete) {
        remove_breakpoint_at_address(addr);
    }

}





std::vector<symbol> debugger::lookup_symbol(const std::string& name) {
    std::vector<symbol> syms;
    for (auto &sec : m_elf.sections()) {
        if (sec.get_hdr().type != elf::sht::symtab && sec.get_hdr().type != elf::sht::dynsym)
            continue;

        for (auto sym : sec.as_symtab()) {
            if (sym.get_name() == name) {
                auto &d = sym.get_data();
                syms.push_back(symbol{to_symbol_type(d.type()), sym.get_name(), d.value});
            }
        }
    }
    return syms;
}





void debugger::dwarf_function_information(const std::string& file_name) {
    std::ofstream write_file;
    write_file.open(file_name, std::ios::app);
    const std::vector<dwarf::compilation_unit> &uints = m_dwarf.compilation_units();
    std::intptr_t pc = get_current_pc_offset_address();
    for (const auto& cu : uints) {
        // Compilation Unit包含多个IDEs，如果pc在某个CU中，则需要遍历该Cu的IDEs，判断其tag
        if (dwarf::die_pc_range(cu.root()).contains(pc)) {
            for (const auto& die : cu.root()) {
                // std::cout << die.tag << std::endl;
                if (die.tag == dwarf::DW_TAG::subprogram) {
                    // std::cout << dwarf::at_name(die) << std::endl;

                    // function的IDE的tag一定是subprogram，但是
                    // 库函数是不包含 DW_AT_Low_pc的，需要添加
                    // 一个条件进行判断。
                    if (die.has(dwarf::DW_AT::prototyped)) {
                        continue;
                    }

                    std::cout << dwarf::at_name(die) << " "
                              << std::hex << dwarf::at_low_pc(die)
                              << "\t" << dwarf::at_high_pc(die) << std::endl;
                }
            }
        }
    }
    write_file.close();
}



/**
 * @brief: 打印当前函数所在堆栈链
 */
void debugger::print_backtrace() {
    // 使用 Lambda 表达式定义一个匿名函数，用于打印堆栈信息
    auto output_frame = [frame_number = 0] (auto&& func) mutable {
        std::cout << "frame #" << frame_number++ << ":0x" << dwarf::at_low_pc(func)
                  << " " << dwarf::at_name(func) << std::endl;
    };

    // 通过去偏置后的地址找到对应的函数DIE
    auto current_func = get_function_from_pc(offset_load_address(get_pc()));
    output_frame(current_func);

    // 获取堆栈起始地址与返回地址
    auto frame_pointer = get_register_value(m_pid, reg_x86_64::rbp);
    auto return_address = read_memory(frame_pointer + 8);

    // 打印函数栈信息，直到[main]函数
    while (dwarf::at_name(current_func) != "main") {

        // std::cout << "\t<debug>: return_address - 0x" << std::hex << return_address << std::endl;
        current_func = get_function_from_pc(offset_load_address(return_address));
        output_frame(current_func);

        // 更新栈指针
        frame_pointer = read_memory(frame_pointer);
        return_address = read_memory(frame_pointer + 8);
    }
}




void debugger::read_variables() {
    using namespace dwarf;

    // 找到当前所在行数
    auto func = get_function_from_pc(get_current_pc_offset_address());

    // 在函数的DIE中遍历[entries]，寻找[variables]
    for (const auto& die : func) {
        if (die.tag == DW_TAG::variable) {
            auto loc_val = die[DW_AT::location];

            if (loc_val.get_type() == value::type::exprloc) {
                ptrace_expr_context context {m_pid};

                // Ask [libelfin] to evaluate the expression for us
                auto result = loc_val.as_exprloc().evaluate(&context);

                switch (result.location_type) {
                    case expr_result::type::address: {
                        auto value = read_memory(result.value);
                        std::cout << at_name(die) << " (0x" << std::hex << result.value << ") ="
                                  << value << std::endl;
                    }

                    case expr_result::type::reg: {
                        auto value = get_register_value_from_dwarf_register(m_pid, result.value);
                        std::cout << at_name(die)<< " (reg" << result.value << ") = "
                                  << value << std::endl;
                        break;
                    }

                    default:
                        throw std::runtime_error{"Unhandled variable location"};
                }
            } 
        }
    }


}







