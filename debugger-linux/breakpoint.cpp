#include "breakpoint.h"
#include <iostream>


void breakpoint::bp_enable() {
  auto data = ptrace(PTRACE_PEEKDATA, m_pid, m_addr, nullptr);  // 从子进程中读取数据
  m_saved_data = static_cast<uint8_t>(data & 0xff); // 保存最低字节
  uint64_t int3 = 0xcc;
  uint64_t data_with_int3 = ((data & ~0xff) | int3);
  ptrace(PTRACE_POKEDATA, m_pid, m_addr, data_with_int3);

  m_enabled = true;
}

void breakpoint::bp_disable() {
  auto data = ptrace(PTRACE_PEEKDATA, m_pid, m_addr, nullptr);
  auto restored_data = ((data & ~0xff) | m_saved_data);
  ptrace(PTRACE_POKEDATA, m_pid, m_addr, restored_data);
  m_enabled = false;
  
};


