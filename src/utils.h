#pragma once

#include <cstdint>

#define _PADDING(n) uint32_t _PAD(__LINE__)[n]
#define _PAD(lnum) _JOIN(_pad, lnum)
#define _JOIN(x, y) x##y

inline constexpr void write32(volatile uint32_t &reg, uint32_t val)
{
  *(volatile uint32_t *)&reg = val;
}

