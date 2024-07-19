#include "cnukkit/server.h"
#include <stdio.h>

int server_handle_packet(Server *server, Frame *frame, Byte *body, DataPacket *packet) {
  switch(body[0]) {
    case GAME_ResourcePackInfo:
    if (server_resource_pack_info(server, frame, body, packet)) return 1;
    break;

    default:
    (void) printf("Unhandled game packet: %#hhx\n", body[0]);
    return 1;
  }

  return 0;
}
