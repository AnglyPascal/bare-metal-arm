#include "uart_pl011.h"
#include "gic.h"

#include <math.h>

namespace uart {
static registers_t *uart0 = (registers_t *)0x10009000u;
static const uint32_t refclock = 24000000u; /* 24 MHz */

error_t init(void) {
  return error_t::UART_OK;
}

error_t configure(config_t *config) {
  /* Validate config */
  if (config->data_bits < 5u || config->data_bits > 8u)
    return error_t::UART_INVALID_ARGUMENT_WORDSIZE;

  if (config->stop_bits == 0u || config->stop_bits > 2u)
    return error_t::UART_INVALID_ARGUMENT_STOP_BITS;

  if (config->baudrate < 110u || config->baudrate > 460800u)
    return error_t::UART_INVALID_ARGUMENT_BAUDRATE;

  /* Disable the UART */
  uart0->CR &= ~CR_UARTEN;

  /* Finish any current transmission, and flush the FIFO */
  while (uart0->FR & FR_BUSY)
    ;
  uart0->LCR_H &= ~LCR_H_FEN;

  /* Set baudrate */
  double intpart, fractpart;
  double baudrate_divisor = (double)refclock / (16u * config->baudrate);
  fractpart = modf(baudrate_divisor, &intpart);

  uart0->IBRD = (uint16_t)intpart;
  uart0->FBRD = (uint8_t)((fractpart * 64u) + 0.5);

  uint32_t lcr_h = 0u;

  /* Set data word size */
  switch (config->data_bits) {
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
  if (config->parity) {
    lcr_h |= LCR_H_PEN;
    lcr_h |= LCR_H_EPS;
    lcr_h |= LCR_H_SPS;
  } else {
    lcr_h &= ~LCR_H_PEN;
    lcr_h &= ~LCR_H_EPS;
    lcr_h &= ~LCR_H_SPS;
  }

  /* Set stop bits */
  if (config->stop_bits == 1u) {
    lcr_h &= ~LCR_H_STP2;
  } else if (config->stop_bits == 2u) {
    lcr_h |= LCR_H_STP2;
  }

  /* Enable FIFOs */
  lcr_h |= LCR_H_FEN;

  uart0->LCR_H = lcr_h;

  uart0->IMSC |= IMSC_RXIM;

  /* Enable the UART */
  uart0->CR |= CR_UARTEN;

  return error_t::UART_OK;
}

void putchar(char c) {
  while (uart0->FR & FR_TXFF)
    ;
  uart0->DR = c;
}

void write(const char *data) {
  while (*data) {
    putchar(*data++);
  }
}

error_t getchar(char *c) {
  if (uart0->FR & FR_RXFE) {
    return error_t::UART_NO_DATA;
  }

  *c = uart0->DR & DR_DATA_MASK;
  if (uart0->RSRECR & RSRECR_ERR_MASK) {
    /* The character had an error */
    uart0->RSRECR &= RSRECR_ERR_MASK;
    return error_t::UART_RECEIVE_ERROR;
  }
  return error_t::UART_OK;
}

/* void __attribute__((interrupt)) isr(void) { */
/*   uint16_t irq = gic_acknowledge_interrupt(); */
/*   if (irq == 37) { */
/*     write("Interrupted\n"); */
/*   } */
/*   uart0->ICR = ICR_ALL_MASK; */
/*   gic_end_interrupt(37); */
/* } */
} // namespace uart
