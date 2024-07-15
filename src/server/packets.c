#include "cnukkit/server.h"
#include "cnukkit/packet.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

static struct RawPacket packet;

int server_packets(const struct Server *server) {
  while (1) {
    packet.length = recvfrom(
      server->socket, packet.data, 
      MAX_PACKET_SIZE, 0, 
      &packet.address, &packet.address_size);

    if (packet.length == -1) {
      // printf("ERRNO: %d | %s\n", errno, strerror(errno));
      continue;
    }

    (void) printf("Packet: %#x\n", packet.data[0]);

    switch(packet.data[0]) {
      case PT_UnconnectedPing:
      server_unconnected_ping(server, &packet);
      break;

      // case PT_OpenConnectionRequest1:
      // server_oc_request1(server, &packet);
      // break;

      default:
      (void) printf("Unhandled packet: %#x\n", packet.data[0]);
      exit(0);
      break;
    }
  }

  return 0;
}
