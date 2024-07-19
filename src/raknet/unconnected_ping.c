#include "cnukkit/raknet.h"
#include "cnukkit/raktypes.h"

#include <alloca.h>
#include <string.h>
#include <sys/socket.h>

#pragma pack(1)
struct UnconnectedPing {
  char id;
  Long time;
  Magic magic;
  Long guid;
};

typedef struct UnconnectedPing UnconnectedPing;

struct UnconnectedPong {
  Byte id;
  Long time;
  Long guid;
  Magic magic;
  ushort motd_length;
  char motd[];
};

typedef struct UnconnectedPong UnconnectedPong;

int raknet_unconnected_ping(Server *server, DataPacket *packet) {
  if (packet->length != sizeof(UnconnectedPing)) return 1;

  const UnconnectedPing rdata = *(UnconnectedPing *) packet->data;

  UnconnectedPong *sdata = (UnconnectedPong *) packet->data;
  sdata->id = RAW_UnconnectedPong;
  sdata->time = rdata.time;
  sdata->guid = GUID;
  sdata->magic[0] = rdata.magic[0];
  sdata->magic[1] = rdata.magic[1];
  sdata->motd_length = htons(server->motd.length);

  (void) memcpy(sdata->motd, server->motd.data, server->motd.length);

  packet->length = sizeof(*sdata) + server->motd.length;

  Packet *spacket;
  if (packet_create(packet, &spacket)) return 2;

  spacket->type = RAW_UnconnectedPong;

  if (pqueue_push(&server->queue, spacket)) return 3;

  return 0;
}
