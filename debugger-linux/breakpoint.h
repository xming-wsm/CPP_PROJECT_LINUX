#ifndef _BREAKPOINT_H
#define _BREAKPOINT_H


#include <pthread.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
// #include <sstream>
#include <cstdint>
#include <stdint.h>
#include <sys/ptrace.h>


class breakpoint {
public:
    breakpoint(pid_t pid, std::intptr_t addr) 
        : m_pid{pid}, m_addr{addr}, m_enabled{false}, m_saved_data{}
    {}

    // Enalbe, set a breakpoint iat address [m_addr] of process [m_pid] and save the data
    void bp_enable();
    // Delete the breakpoint and restore the data
    void bp_disable();

    // is this object has an active breakpoint
    auto is_enabled() const -> bool {return m_enabled;}
    // return the address of the breakpoint
    auto get_address() const -> std::intptr_t {return m_addr;}



private:
    pid_t m_pid;
    std::intptr_t m_addr;
    bool m_enabled;
    uint8_t m_saved_data; // breakpoint地址原本的数据
};



#endif /* BREAKPOINT_H */
