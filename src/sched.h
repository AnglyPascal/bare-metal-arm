#pragma once

#include "systime.h"

namespace sched
{

using task_entry_ptr_t = void (*)(void);

struct task_t {
  task_entry_ptr_t entry;
  sys::time_t period;
  sys::time_t last_run;
};

enum class error_t {
  SCHED_OK,
  SCHED_TOO_MANY_TASKS,
};

constexpr auto MAX_NUM_TASKS = 8u;

error_t add_task(task_entry_ptr_t entry, sys::time_t period);
void run(void);

} // namespace sched

