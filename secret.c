#include <stdint.h>
#include <stdio.h>

void do_something_special(uint8_t* value, size_t size)
{
  uint64_t result = 0;

  for (uint8_t* i = value; i < value + size; i++) {
    result += *i;
  }

  printf("result = %lld\n", result);
}
