#include "cpu.h"
#include "gic.h"
#include "uart_pl011.h"

/* #include <stdbool.h> */
/* #include <stdint.h> */
/* #include <string.h> */

int main() {
  uart_config_t config = {
      .data_bits = 8,
      .stop_bits = 1,
      .parity = false,
      .baudrate = 9600,
  };
  uart_configure(&config);

  uart_putchar('A');
  uart_putchar('B');
  uart_putchar('C');
  uart_putchar('\n');

  uart_write("input:\n");

  gic_init();
  gic_enable_interrupt(UART0_INTERRUPT);
  cpu_enable_interrupts();

  while (1)
    ;

  return 0;
}
