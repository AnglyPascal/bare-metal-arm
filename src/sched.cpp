#include "sched.h"
#include "uart_lib.h"

namespace sched
{

static task_t task_table[MAX_NUM_TASKS] = {nullptr, 0, 0};
static uint8_t table_idx = 0;

error_t add_task(task_entry_ptr_t entry, sys::time_t period)
{
  if (table_idx == MAX_NUM_TASKS) {
    return error_t::SCHED_TOO_MANY_TASKS;
  }

  task_table[table_idx++] = {entry, period, 0};
  return error_t::SCHED_OK;
}

void run(void)
{
  while (1) {
    for (uint8_t i = 0; i < MAX_NUM_TASKS; i++) {
      auto &task = task_table[i];
      if (task.entry == nullptr) {
        continue;
      }

      if (task.period <= sys::curr_time() - task.last_run) {
        task.last_run = sys::curr_time();
        task.entry();
      }
    }
  }
}

} // namespace sched
