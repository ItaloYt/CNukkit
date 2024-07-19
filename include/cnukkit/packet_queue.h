#ifndef CNUKKIT_PACKET_QUEUE_H
#define CNUKKIT_PACKET_QUEUE_H

#include "cnukkit/packet.h"

struct PacketQueue {
  const struct Packet **packet;
  unsigned length, total;
};

typedef struct PacketQueue PacketQueue;

int pqueue_create(struct PacketQueue *queue);
int pqueue_push(struct PacketQueue *queue, const struct Packet *packet);
void pqueue_reorder(struct PacketQueue *queue, unsigned remaining);
void pqueue_clear(struct PacketQueue *queue);

#endif
