#include <iostream>
#include <string>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/personality.h>

#include "debugger.h"


using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Program name not specified";
        return -1;
    }

    auto prog = argv[1];

    auto pid = fork();
    if (pid == 0) {
        personality(ADDR_NO_RANDOMIZE);
        ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);

        // 由于调用了ptrace函数，子进程调用execl函数后状态发生变化，会马上告知监视的父进程
        execl(prog, prog, nullptr);
    }

    else if (pid >= 1)  {
        // 父进程可以监控子进程
        cout << "开始调试的进程ID: " << pid << endl;
        debugger dbg {prog, pid};
        dbg.run();
    }
}
