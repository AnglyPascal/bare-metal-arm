#pragma once

#include "utils.h"
#include <cstdint>

namespace uart
{

struct __attribute__((packed)) registers_t {
  uint32_t DR;       /* 0x0 Data Register */
  uint32_t RSRECR;   /* 0x4 Receive status / error clear register */
  _PADDING(4);       /* 0x8 - 0x14 reserved */
  const uint32_t FR; /* 0x18 Flag register */
  _PADDING(1);       /* 0x1C reserved */
  uint32_t ILPR;     /* 0x20 Low-power counter register */
  uint32_t IBRD;     /* 0x24 Integer baudrate register */
  uint32_t FBRD;     /* 0x28 Fractional baudrate register */
  uint32_t LCR_H;    /* 0x2C Line control register */
  uint32_t CR;       /* 0x30 Control register */
  uint32_t IFLS;     /* 0x34 FIFO level select register */
  uint32_t IMSC;     /* 0x38 Interrupt mask set/clear register */
  uint32_t RIS;      /* 0x3C Raw interrupt status register */
  uint32_t MIS;      /* 0x40 Masked interrupt status register */
  uint32_t ICR;      /* 0x44 Interrupt clear register */
};

enum class error_t {
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

constexpr auto UART0 = 0x10009000u;
constexpr auto REF_CLOCK = 24000000u; /* 24 MHz */

constexpr auto UART0_INTERRUPT = 37u;

constexpr auto DR_DATA_MASK = 0xFFu;

constexpr auto ECR_BE = 1 << 2u;

constexpr auto FR_BUSY = 1 << 3u;
constexpr auto FR_RXFE = 1 << 4u;
constexpr auto FR_TXFF = 1 << 5u;

constexpr auto RSRECR_ERR_MASK = 0xFu;

constexpr auto LCR_H_FEN = 1 << 4u;
constexpr auto LCR_H_PEN = 1 << 1u;
constexpr auto LCR_H_EPS = 1 << 2u;
constexpr auto LCR_H_STP2 = 1 << 3u;
constexpr auto LCR_H_SPS = 1 << 7u;
constexpr auto CR_UARTEN = 1 << 0u;

constexpr auto LCR_H_WLEN_5BITS = 0u << 5u;
constexpr auto LCR_H_WLEN_6BITS = 1u << 5u;
constexpr auto LCR_H_WLEN_7BITS = 2u << 5u;
constexpr auto LCR_H_WLEN_8BITS = 3u << 5u;

constexpr auto IFLS_RXFL_1_8 = 0u << 5u;
constexpr auto IFLS_TXFL_1_8 = 0u << 2u;

constexpr auto IMSC_RXIM = 1u << 4u;
constexpr auto IMSC_TXIM = 1u << 5u;

constexpr auto RX_INTERRUPT = 1u << 4u;
constexpr auto BE_INTERRUPT = 1u << 9u;

constexpr auto ICR_ALL_MASK = 0x7FFu;

error_t configure(config_t &config);
void putchar(char c);
void write(const char *data);
error_t getchar(char &c);
/* void isr(void); */

} // namespace uart
