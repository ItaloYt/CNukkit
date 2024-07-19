#include "cnukkit/raknet.h"

int raknet_is_recorded(Server *server, const triad sequence) {
  for (unsigned index = recovery_index, count = 0; count < RECOVERY_LENGTH; ++count, index = (index - 1) % 10) {
    if (sequence[0] == recovery[index][0] && sequence[1] == recovery[index][1] && sequence[2] == recovery[index][2])
      return 1;
  }
  
  return 0;
}
