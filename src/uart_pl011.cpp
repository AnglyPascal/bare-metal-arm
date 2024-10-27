#include "uart_pl011.h"
#include "hardware.h"
#include <math.h>

namespace uart
{

static volatile registers_t *const uart0 = (uart::registers_t *)UART0;

error_t configure(config_t &config)
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
  uart0->LCRH &= ~LCRH_FEN;

  /* Set baudrate */
  double intpart, fractpart;
  double baudrate_divisor = (double)REF_CLOCK / (16u * config.baudrate);
  fractpart = modf(baudrate_divisor, &intpart);

  uart0->IBRD = (uint16_t)intpart;
  uart0->FBRD = (uint8_t)((fractpart * 64u) + 0.5);

  uint32_t lcrh = 0u;

  /* Set data word size */
  switch (config.data_bits) {
  case 5:
    lcrh |= LCRH_WLEN_5BITS;
    break;
  case 6:
    lcrh |= LCRH_WLEN_6BITS;
    break;
  case 7:
    lcrh |= LCRH_WLEN_7BITS;
    break;
  case 8:
    lcrh |= LCRH_WLEN_8BITS;
    break;
  }

  /* Set parity. If enabled, use even parity */
  if (config.parity) {
    lcrh |= LCRH_PEN;
    lcrh |= LCRH_EPS;
    lcrh |= LCRH_SPS;
  } else {
    lcrh &= ~LCRH_PEN;
    lcrh &= ~LCRH_EPS;
    lcrh &= ~LCRH_SPS;
  }

  /* Set stop bits */
  if (config.stop_bits == 1u) {
    lcrh &= ~LCRH_STP2;
  } else if (config.stop_bits == 2u) {
    lcrh |= LCRH_STP2;
  }

  /* Enable FIFOs */
  lcrh |= LCRH_FEN;

  uart0->LCRH = lcrh;

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

} // namespace uart
