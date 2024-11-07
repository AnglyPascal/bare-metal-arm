#pragma once
#include "uart_pl011.h"

namespace uart
{

void write_uint(uint32_t n);
void write_int(int32_t n);

} // namespace uart
