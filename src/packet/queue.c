#include "cnukkit/packet_queue.h"
#include <stdio.h>
#include <stdlib.h>

void pqueue_reorder(PacketQueue *queue, unsigned remaining) {
  int space_index = 0;

  for (int index = 0; index < queue->length; ++index) {
    if (!queue->packet[index]) {
      space_index = index;
      continue;
    }

    if (index < remaining || space_index == -1) continue;
    queue->packet[space_index] = queue->packet[index];
    queue->packet[index] = 0x0;

    index = space_index - 1;

    space_index = -1;
  }
}

int pqueue_push(PacketQueue *queue, const Packet *packet) {
  if (queue->length == queue->total) {
    queue->packet = realloc(queue->packet, ++queue->total * sizeof(*queue->packet));

    (void) printf("Queue Total: %u\n", queue->total);

    if (!queue->packet) return 1;
  }

  queue->packet[queue->length++] = packet;

  return 0;
}

void pqueue_clear(PacketQueue *queue) {
  for (unsigned index = 0; index < queue->length; ++index) {
    packet_free(queue->packet[index]);
  }

  free(queue->packet);
  queue->total = 0;
}

int pqueue_create(PacketQueue *queue) {
  queue->length = queue->total = 0;
  queue->packet = malloc(queue->total * sizeof(*queue->packet));

  if (!queue->packet) return 1;

  return 0;
}
