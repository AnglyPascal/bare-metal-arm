#include "gic.h"
#include "utils.h"

namespace gic
{

static volatile distributor_registers_t *dregs;
static volatile cpu_interface_registers_t *ifregs;

void init(void)
{
  const auto GIC_DIST_BASE = cpu::get_periphbase() + GIC_DISTRIBUTOR_OFFSET;
  const auto GIC_IFACE_BASE = cpu::get_periphbase() + GIC_IFACE_OFFSET;

  ifregs = (cpu_interface_registers_t *)GIC_IFACE_BASE;
  dregs = (distributor_registers_t *)GIC_DIST_BASE;

  // Enable all interrupt priorities
  write32(ifregs->CCPMR, 0xFFFFu);
  // Enable interrupt forwarding to this CPU
  write32(ifregs->CCTLR, CCTRL_ENABLE);

  write32(dregs->DCTLR, DCTRL_ENABLE); /* Enable the interrupt distributor */
}

void enable_interrupt(uint16_t number)
{
  /* Enable the interrupt */
  uint8_t reg = number / 32;
  uint8_t bit = number % 32;

  uint32_t reg_val = dregs->DISENABLER[reg];
  reg_val |= (1u << bit);
  write32(dregs->DISENABLER[reg], reg_val);

  /* Forward interrupt to CPU Interface 0 */
  reg = number / 4;
  bit = (number % 4) * 8; /* Can result in bit 0, 8, 16 or 24 */
  reg_val = dregs->DITARGETSR[reg];
  reg_val |= (1u << bit);
  write32(dregs->DITARGETSR[reg], reg_val);
}

} // namespace gic
