#include "cnukkit/raknet.h"
#include "cnukkit/util.h"

#pragma pack(1)
struct ConnectedPing {
  Byte id;
  Long time;
};

typedef struct ConnectedPing ConnectedPing;

struct ConnectedPong {
  Byte id;
  Long ping;
  Long pong;
};

typedef struct ConnectedPong ConnectedPong;

int raknet_connected_ping(Server *server, Frame *frame, Byte *body, DataPacket *packet) {
  ConnectedPing rdata = *(ConnectedPing *) body;

  ConnectedPong sdata = {
    .id = RAW_ConnectedPong,
    .ping = rdata.time,
    .pong = get_time_milliseconds()
  };

  Frame sframe = {
    .id = RAW_FrameSet,
    .sequence_number = { 0, 0, 1 },
    .flags = RAW_UNRELIABLE,
    .length = sizeof(sdata) * 8
  };

  struct PacketBuffer buffer = {
    .data = packet->data,
    .pointer = packet->data
  };

  pb_put_frame(&buffer, &sframe);
  pb_put_struct(&buffer, &sdata, sizeof(sdata));

  packet->length = buffer.pointer - buffer.data;

  struct Packet *spacket;
  if (packet_create(packet, &spacket)) return 1;

  spacket->type = RAW_ConnectedPong;

  if (pqueue_push(&server->queue, spacket)) return 2;

  return 0;
}
