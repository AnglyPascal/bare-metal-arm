#pragma once

#include <cstdint>

namespace cpu
{

constexpr auto REF_CLOCK = 24000000u; /* 24 MHz */

inline uint32_t get_periphbase(void);
inline void enable_interrupts(void);

inline uint32_t get_periphbase(void)
{
  uint32_t result;
  asm("mrc p15, #4, %0, c15, c0, #0" : "=r"(result));
  return result;
}

inline void enable_interrupts(void)
{
  asm("cpsie if");
}

} // namespace cpu
