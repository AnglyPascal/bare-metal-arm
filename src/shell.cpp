#include "shell.h"

#include "uart_pl011.h"
#include <cstring>

void shell::parse_cmd()
{
  if (!strncmp("help\r", buf, strlen("help\r"))) {
    uart::write("Just type and see what happens!\n");
  } else if (!strncmp("uname\r", buf, strlen("uname\r"))) {
    uart::write("bare-metal arm 06_uart\n");
  }
}

void shell::run()
{
  while (1) {
    char c;
    if (uart::getchar(c) == uart::error_t::UART_OK) {
      uart::putchar(c);
      buf[buf_idx % 64] = c;
      buf_idx++;
      if (c == '\r') {
        uart::write("\n");
        buf_idx = 0u;
        parse_cmd();
      }
    }
  }
}
