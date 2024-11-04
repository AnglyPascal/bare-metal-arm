#pragma once

#include "utils.h"
#include <cstdint>

namespace ptimer
{

#pragma pack(push, 1)
struct registers_t {
  uint32_t LR;
  uint32_t CR;
  uint32_t CTRL;
  uint32_t ISR;
};
#pragma pack(pop)

constexpr auto PTIMER0_OFFSET = 0x600u;
constexpr auto PTIMER0_INTERRUPT = 29u;

constexpr auto CTRL_EN = bit(0);
constexpr auto CTRL_AUTORELOAD = bit(1);
constexpr auto CTRL_IRQ_EN = bit(2);

constexpr auto ISR_CLEAR = bit(0);

enum class error_t {
  PTIMER_OK,
  PTIMER_INVALID_PERIOD,
};

error_t init(uint16_t ms);

} // namespace ptimer

