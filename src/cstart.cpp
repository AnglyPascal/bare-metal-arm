#include "cpu.h"
#include "gic.h"
#include "shell.h"
#include "uart_pl011.h"

#include <cstdint>

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
  cpu::enable_interrupts();

  uart::write("Type below...\n");

  shell sh;
  sh.run();

  return 0;
}
