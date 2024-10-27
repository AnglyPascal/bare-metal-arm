#include "cpu.h"
#include "gic.h"
#include "uart_pl011.h"

/* #include <stdbool.h> */
/* #include <stdint.h> */
/* #include <string.h> */

int main() {
  uart::config_t config = {
      .data_bits = 8,
      .stop_bits = 1,
      .parity = false,
      .baudrate = 9600,
  };
  uart::configure(&config);

  uart::putchar('A');
  uart::putchar('B');
  uart::putchar('C');
  uart::putchar('\n');

  uart::write("input:\n");

  gic::gic_init();
  gic::gic_enable_interrupt(UART0_INTERRUPT);
  cpu::cpu_enable_interrupts();

  while (1)
    ;

  return 0;
}
