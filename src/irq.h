#pragma once

#include <cstdint>

namespace irq
{

using isr_ptr_t = void (*)(void);

constexpr auto ISR_COUNT = 1024;
constexpr auto MAX_ISR = ISR_COUNT - 1;

enum class error_t { IRQ_OK, IRQ_INVALID_IRQ_ID, IRQ_ALREADY_REGISTERED };

error_t register_isr(uint16_t irq_num, isr_ptr_t callback);

} // namespace irq
