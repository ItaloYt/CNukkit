#include "cnukkit/raknet.h"
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>

#pragma pack(1)
struct OpenConnectionRequest2 {
  Byte id;
  Magic magic;
  Address address;
  short mtu;
  Long guid;
};

typedef struct OpenConnectionRequest2 OpenConnectionRequest2;

struct OpenConnectionReply2 {
  Byte id;
  Magic magic;
  Long guid;
  Address address;
  short mtu;
  Bool encryption;
};

typedef struct OpenConnectionReply2 OpenConnectionReply2;

int raknet_oc_request2(Server *server, DataPacket *packet) {
  if (packet->length != sizeof(OpenConnectionRequest2)) return 1;

  OpenConnectionRequest2 rdata = *(OpenConnectionRequest2 *) packet->data;

  OpenConnectionReply2 *sdata = (OpenConnectionReply2 *) packet->data;
  sdata->id = RAW_OpenConnectionReply2;
  // Magic number is already there
  // sdata->magic[0] = rdata.magic[0];
  // sdata->magic[1] = rdata.magic[1];
  sdata->guid = GUID;
  sdata->address[0] = 4;
  *(unsigned *) (sdata->address + 1) = packet->address.sin_addr.s_addr;
  *(ushort *) (sdata->address + 5) = packet->address.sin_port;
  sdata->mtu = rdata.mtu;
  sdata->encryption = 0;

  packet->length = sizeof(OpenConnectionReply2);

  Packet *spacket;
  if (packet_create(packet, &spacket)) return 2;

  spacket->type = RAW_OpenConnectionReply2;

  if (pqueue_push(&server->queue, spacket)) return 3;

  return 0;
}
