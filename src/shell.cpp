#include "shell.h"

#include "uart_pl011.h"
#include <cstring>

namespace shell
{

void parse_cmd(const char *cmd)
{
  if (!strncmp("help\r", cmd, strlen("help\r"))) {
    uart::write("Just type and see what happens!\n");
  } else if (!strncmp("uname\r", cmd, strlen("uname\r"))) {
    uart::write("bare-metal arm 06_uart\n");
  } else {
    uart::write("unknown command\n");
  }
}

void init()
{
  uart::register_cmd_handler(parse_cmd);
}

void run()
{
  while (1)
    ;
}

} // namespace shell
