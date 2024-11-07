#include "uart_lib.h"

namespace uart
{

void write_uint(uint32_t n)
{
  uint8_t digits[10] = {0};

  auto i = 0;
  do {
    digits[i++] = '0' + (n % 10);
    n /= 10;
  } while (n > 0);

  while (i > 0) {
    putchar(digits[--i]);
  }
}

void write_int(int32_t n)
{
  if (n < 0) {
    putchar('-');
    write_uint(-n);
  } else {
    write_uint(n);
  }
}

} // namespace uart
