#include "ptimer.h"
#include "cpu.h"
#include "irq.h"
#include "systime.h"
#include "uart_pl011.h"
#include "utils.h"

namespace ptimer
{

static volatile registers_t *regs;

void isr(void);

/* The private timer's period is:
 *
 * (Prescaler + 1) * (Load value + 1)
 * ----------------------------------
 *             Refclock
 *
 * Simplified, if prescaler == 0, then:
 *   Load value = (period * refclock) - 1
 */

static bool validate_config(uint16_t ms)
{
  /* Very simplified - assume we leave prescaler at 0
   * Then the largest possible numerator is just UINT32_MAX */
  uint32_t max_period = UINT32_MAX / cpu::REF_CLOCK;
  uint32_t max_ms = max_period * 1000u;

  return ms < max_ms;
}

uint32_t ms_to_timer(uint16_t ms)
{
  double period = ms * 0.001;
  auto value = (period * cpu::REF_CLOCK) - 1;
  value *= 3; // additional QEMU slowdown factor
  return value;
}

error_t init(uint16_t ms)
{
  const uint64_t PTIMER0 = cpu::get_periphbase() + PTIMER0_OFFSET;
  regs = (volatile registers_t *)PTIMER0;

  if (!validate_config(ms))
    return error_t::PTIMER_INVALID_PERIOD;

  uint32_t load_val = ms_to_timer(ms);
  write32(regs->LR, load_val);

  irq::register_isr(PTIMER0_INTERRUPT, isr);

  uint32_t ctrl = CTRL_EN | CTRL_AUTORELOAD | CTRL_IRQ_EN;
  write32(regs->CTRL, ctrl);

  return error_t::PTIMER_OK;
}

void isr(void)
{
  /* uart::write("timer!\n"); */
  write32(regs->ISR, ISR_CLEAR);
  sys::time_tick();
}

} // namespace ptimer
