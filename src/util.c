#include "cnukkit/util.h"
#include <time.h>

Long get_time_milliseconds() {
  struct timespec spec;
  clock_gettime(CLOCK_MONOTONIC, &spec);
  
  return (Long) spec.tv_sec * 1000 + spec.tv_nsec / 1000000;
}
