#pragma once

#include <cstdint>

namespace sys
{

/* struct time_t { */
/*   uint32_t time; */
/* }; */

using time_t = uint32_t;

void time_tick();
time_t curr_time();

} // namespace sys
