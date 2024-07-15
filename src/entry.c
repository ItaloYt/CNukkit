#include "cnukkit/server.h"

#include <stdio.h>

struct Server server = {
  .title = stringl("Titulo"),
  .protocol = 686,
  .version = stringl("1.21.2"),
  .player_count = 0,
  .player_maximum = 10,
  .subtitle = stringl("Subtitulo")
};

int main() {
  (void) puts("starting server");

  setbuf(stdout, 0x0);

  if (server_start(&server)) return 1;

  if (server_packets(&server)) return 2;

  if (server_close(&server)) return 3;

  return 0;
}
