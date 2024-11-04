#include "systime.h"

namespace sys
{

static volatile time_t systime = 0;

void time_tick()
{
  systime++;
}

time_t curr_time()
{
  return systime;
}

} // namespace sys
