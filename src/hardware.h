#pragma once

#include <cstdint>

namespace uart
{

constexpr auto DR_DATA_MASK = 0xFFu;

constexpr auto FR_BUSY = 1 << 3u;
constexpr auto FR_RXFE = 1 << 4u;
constexpr auto FR_TXFF = 1 << 5u;

constexpr auto RSRECR_ERR_MASK = 0xFu;

constexpr auto LCRH_FEN = 1 << 4u;
constexpr auto LCRH_PEN = 1 << 1u;
constexpr auto LCRH_EPS = 1 << 2u;
constexpr auto LCRH_STP2 = 1 << 3u;
constexpr auto LCRH_SPS = 1 << 7u;
constexpr auto CR_UARTEN = 1 << 0u;

constexpr auto LCRH_WLEN_5BITS = 0u << 5u;
constexpr auto LCRH_WLEN_6BITS = 1u << 5u;
constexpr auto LCRH_WLEN_7BITS = 2u << 5u;
constexpr auto LCRH_WLEN_8BITS = 3u << 5u;

constexpr auto REF_CLOCK = 24000000u; /* 24 MHz */
constexpr auto UART0 = 0x10009000u;

struct __attribute__((packed)) registers_t {
  uint32_t DR;            /* 0x0 Data Register */
  uint32_t RSRECR;        /* 0x4 Receive status / error clear register */
  uint32_t _reserved0[4]; /* 0x8 - 0x14 reserved */
  const uint32_t FR;      /* 0x18 Flag register */
  uint32_t _reserved1;    /* 0x1C reserved */
  uint32_t ILPR;          /* 0x20 Low-power counter register */
  uint32_t IBRD;          /* 0x24 Integer baudrate register */
  uint32_t FBRD;          /* 0x28 Fractional baudrate register */
  uint32_t LCRH;          /* 0x2C Line control register */
  uint32_t CR;            /* 0x30 Control register */
};

} // namespace uart
