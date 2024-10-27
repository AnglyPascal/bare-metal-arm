#pragma once

#include <cstdint>

namespace uart
{

enum struct error_t {
  UART_OK = 0,
  UART_INVALID_ARGUMENT_BAUDRATE,
  UART_INVALID_ARGUMENT_WORDSIZE,
  UART_INVALID_ARGUMENT_STOP_BITS,
  UART_RECEIVE_ERROR,
  UART_NO_DATA
};

struct config_t {
  uint8_t data_bits;
  uint8_t stop_bits;
  bool parity;
  uint32_t baudrate;
};

error_t configure(config_t &config);
void putchar(char c);
void write(const char *data);
error_t getchar(char &c);

} // namespace uart

