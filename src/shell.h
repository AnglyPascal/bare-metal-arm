#pragma once

#include <cstdint>

class shell
{
private:
  char buf[64];
  uint8_t buf_idx = 0u;

  void parse_cmd();

public:
  void run();
};
