#include "uart_pl011.h"
#include <cstdint>
#include <cstring>

class shell
{
private:
  char buf[64];
  uint8_t buf_idx = 0u;

  void parse_cmd()
  {
    if (!strncmp("help\r", buf, strlen("help\r"))) {
      uart::write("Just type and see what happens!\n");
    } else if (!strncmp("uname\r", buf, strlen("uname\r"))) {
      uart::write("bare-metal arm 06_uart\n");
    }
  }

public:
  void run()
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
};

int main()
{
  uart::config_t config = {
      .data_bits = 8, .stop_bits = 1, .parity = false, .baudrate = 9600};
  uart::configure(config);

  uart::putchar('A');
  uart::putchar('B');
  uart::putchar('C');
  uart::putchar('\n');

  uart::write("I love drivers!\n");
  uart::write("Type below...\n");

  shell sh;
  sh.run();

  return 0;
}
