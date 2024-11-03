#include "uart_pl011.h"
#include "gic.h"
#include "irq.h"
#include <math.h>

namespace uart
{

static volatile registers_t *uart0 = (registers_t *)UART0;

error_t init(void)
{
  return error_t::UART_OK;
}

void isr();

error_t configure(const config_t &config)
{
  /* Validate config */
  if (config.data_bits < 5u || config.data_bits > 8u) {
    return error_t::UART_INVALID_ARGUMENT_WORDSIZE;
  }

  if (config.stop_bits == 0u || config.stop_bits > 2u) {
    return error_t::UART_INVALID_ARGUMENT_STOP_BITS;
  }

  if (config.baudrate < 110u || config.baudrate > 460800u) {
    return error_t::UART_INVALID_ARGUMENT_BAUDRATE;
  }

  /* Disable the UART */
  uart0->CR &= ~CR_UARTEN;

  /* Finish any current transmission, and flush the FIFO */
  while (uart0->FR & FR_BUSY)
    ;
  uart0->LCR_H &= ~LCR_H_FEN;

  /* Set baudrate */
  double intpart, fractpart;
  double baudrate_divisor = (double)REF_CLOCK / (16u * config.baudrate);
  fractpart = modf(baudrate_divisor, &intpart);

  uart0->IBRD = (uint16_t)intpart;
  uart0->FBRD = (uint8_t)((fractpart * 64u) + 0.5);

  uint32_t lcr_h = 0u;

  /* Set data word size */
  switch (config.data_bits) {
  case 5:
    lcr_h |= LCR_H_WLEN_5BITS;
    break;
  case 6:
    lcr_h |= LCR_H_WLEN_6BITS;
    break;
  case 7:
    lcr_h |= LCR_H_WLEN_7BITS;
    break;
  case 8:
    lcr_h |= LCR_H_WLEN_8BITS;
    break;
  }

  /* Set parity. If enabled, use even parity */
  if (config.parity) {
    lcr_h |= LCR_H_PEN;
    lcr_h |= LCR_H_EPS;
    lcr_h |= LCR_H_SPS;
  } else {
    lcr_h &= ~LCR_H_PEN;
    lcr_h &= ~LCR_H_EPS;
    lcr_h &= ~LCR_H_SPS;
  }

  /* Set stop bits */
  if (config.stop_bits == 1u) {
    lcr_h &= ~LCR_H_STP2;
  } else if (config.stop_bits == 2u) {
    lcr_h |= LCR_H_STP2;
  }

  /* Enable FIFOs */
  lcr_h |= LCR_H_FEN;

  uart0->LCR_H = lcr_h;

  /* Enable the UARTRXINTR interrupt, Tb. 3-14 */
  uart0->IMSC |= IMSC_RXIM;
  irq::register_isr(UART0_INTERRUPT, isr);

  /* Enable the UART */
  uart0->CR |= CR_UARTEN;

  return error_t::UART_OK;
}

void putchar(char c)
{
  while (uart0->FR & FR_TXFF)
    ;
  uart0->DR = c;
}

void write(const char *data)
{
  while (*data) {
    putchar(*data++);
  }
}

error_t getchar(char &c)
{
  if (uart0->FR & FR_RXFE) {
    return error_t::UART_NO_DATA;
  }

  c = uart0->DR & DR_DATA_MASK;
  if (uart0->RSRECR & RSRECR_ERR_MASK) {
    /* The character had an error */
    uart0->RSRECR &= RSRECR_ERR_MASK;
    return error_t::UART_RECEIVE_ERROR;
  }
  return error_t::UART_OK;
}

struct buffer_t {
  cmd_handler_t cmd_handler;

  static constexpr auto bufsize = 31;
  char buf[bufsize];
  uint8_t i;

  void push(char c)
  {
    if (i == bufsize - 1)
      i = 0;
    buf[i++] = c;
  }

  void cmd(void)
  {
    buf[i] = '\0';
    cmd_handler(buf);
    i = 0;
  }
};

buffer_t buffer{};

void register_cmd_handler(cmd_handler_t cmd_handler)
{
  buffer.cmd_handler = cmd_handler;
}

void isr()
{
  uint32_t status = uart0->MIS;
  if (status & RX_INTERRUPT) {
    char c = uart0->DR & DR_DATA_MASK;
    putchar(c);
    buffer.push(c);
    if (c == '\r') {
      putchar('\n');
      buffer.cmd();
    }
  } else if (status & BE_INTERRUPT) {
    write("Break error detected\n");
    /* clear the error flag */
    uart0->RSRECR = ECR_BE;
    /* clear the interrupt */
    uart0->ICR = BE_INTERRUPT;
  }
}

} // namespace uart

