#ifndef _PTRACE_EXPR_CONTEXT_H
#define _PTRACE_EXPR_CONTEXT_H
#include "debugger.h"
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
#include <ostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <vector>

#include "libelfin/dwarf/data.hh"
#include "libelfin/dwarf/dwarf++.hh"


class ptrace_expr_context : public dwarf::expr_context {
public:
    ptrace_expr_context(pid_t pid) : m_pid{pid} {}
    
    dwarf::taddr reg (unsigned regnum) override {
        return get_register_value_from_dwarf_register(m_pid, regnum);
    }

    dwarf::taddr pc() {
        struct user_regs_struct regs;
        ptrace(PTRACE_GETREGS, m_pid, nullptr, &regs);
        return regs.rip;
    }

    dwarf::taddr deref_size(dwarf::taddr address, unsigned size) override {
        return ptrace(PTRACE_PEEKDATA, m_pid, address, nullptr);
    }


private:
    pid_t m_pid;
};

















































#endif /* _PTRACE_EXPR_CONTEXT_H */
