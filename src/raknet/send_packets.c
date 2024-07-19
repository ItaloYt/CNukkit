#include "cnukkit/raknet.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

void raknet_send_packets(Server *server) {
  unsigned failed = 0;

  for (unsigned index = 0; index < server->queue.length; ++index) {
    const Packet *packet = server->queue.packet[index];

    if (sendto(server->socket, packet->data, packet->length, 0, &packet->address, packet->address_size) == -1) {
      (void) printf("Packet(type = %u\n) send error: %#hhx %s\n", packet->type, errno, strerror(errno));
      ++failed;
    }

    (void) printf("Sending packet(type = %#hhx)\n", packet->type);

    packet_free(packet);
    server->queue.packet[index] = 0x0;

    continue;
  }

  pqueue_reorder(&server->queue, failed);
  server->queue.length = failed;

  if (failed)
    (void) printf("Packets not send: %u\n", failed);
}
