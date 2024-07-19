#include "cnukkit/raknet.h"
#include <netinet/in.h>
#include <sys/socket.h>

#pragma pack(1)
struct OpenConnectionRequest1 {
  Byte id;
  Magic magic;
  Byte protocol;
  char mtu[];
};

typedef struct OpenConnectionRequest1 OpenConnectionRequest1;

struct OpenConnectionReply1 {
  Byte id;
  Magic magic;
  Long guid;
  Bool use_encryption;
  short mtu;
};

typedef struct OpenConnectionReply1 OpenConnectionReply1;

int raknet_oc_request1(Server *server, DataPacket *packet) {
  if (packet->length < sizeof(OpenConnectionRequest1)) return 1;

  // struct OpenConnectionRequest1 rdata = *(struct OpenConnectionRequest1 *) packet->data;

  server->mtu = packet->length - sizeof(OpenConnectionRequest1) + 28;

  OpenConnectionReply1 *sdata = (OpenConnectionReply1 *) packet->data;
  sdata->id = RAW_OpenConnectionReply1;
  // MAGIC is already set
  // sdata->magic[0] = rdata.magic[0];
  // sdata->magic[1] = rdata.magic[1];
  sdata->guid = GUID;
  sdata->use_encryption = 0;
  sdata->mtu = htons(server->mtu);

  packet->length = sizeof(OpenConnectionReply1);

  Packet *spacket;
  if (packet_create(packet, &spacket)) return 2;

  spacket->type = RAW_OpenConnectionReply1;

  if (pqueue_push(&server->queue, spacket)) return 3;

  return 0;
}
