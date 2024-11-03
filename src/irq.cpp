#include "irq.h"
#include "gic.h"

namespace irq
{

static isr_ptr_t callbacks[ISR_COUNT] = {nullptr};

error_t register_isr(uint16_t irq_num, isr_ptr_t cb)
{
  if (irq_num > MAX_ISR) {
    return error_t::IRQ_INVALID_IRQ_ID;
  }

  if (callbacks[irq_num] != nullptr) {
    return error_t::IRQ_ALREADY_REGISTERED;
  }

  callbacks[irq_num] = cb;
  return error_t::IRQ_OK;
}

extern "C" {
void __attribute__((interrupt)) irq_handler(void *)
{
  uint16_t irq = gic::acknowledge_interrupt();
  isr_ptr_t isr = irq > MAX_ISR ? nullptr : callbacks[irq];
  if (isr != nullptr) {
    isr();
  }
  gic::end_interrupt(irq);
}
}

} // namespace irq

