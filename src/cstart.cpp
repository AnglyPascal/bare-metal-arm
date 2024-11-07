#include "cpu.h"
#include "gic.h"
#include "ptimer.h"
#include "sched.h"
#include "shell.h"
#include "systime.h"
#include "uart_lib.h"
#include "uart_pl011.h"

#include <cstdint>

void task0(void)
{
  auto start = sys::curr_time();
  uart::write("Entering task 0... systime: ");
  uart::write_uint(start);
  uart::putchar('\n');
  while (start + 100u > sys::curr_time())
    ;
  uart::write("Exiting task 0...\n");
}

void task1(void)
{
  auto start = sys::curr_time();
  uart::write("Entering task 1... systime: ");
  uart::write_uint(start);
  uart::putchar('\n');
  while (start + 100u > sys::curr_time())
    ;
  uart::write("Exiting task 1...\n");
}

int main()
{
  uart::config_t config = {
      .data_bits = 8, .stop_bits = 1, .parity = false, .baudrate = 9600};
  uart::configure(config);

  uart::putchar('A');
  uart::putchar('B');
  uart::putchar('C');
  uart::putchar('\n');

  gic::init();
  gic::enable_interrupt(uart::UART0_INTERRUPT);

  gic::enable_interrupt(ptimer::PTIMER0_INTERRUPT);
  if (ptimer::init(10u) != ptimer::error_t::PTIMER_OK)
    uart::write("ptimer setup failed\n");

  cpu::enable_interrupts();
  uart::write("Type below...\n");

  /* shell::init(); */
  /* shell::run(); */
  sched::add_task(task0, 500u);
  sched::add_task(task1, 200u);

  sched::run();

  return 0;
}
