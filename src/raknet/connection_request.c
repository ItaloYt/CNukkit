#include "cnukkit/raknet.h"
#include "cnukkit/util.h"
#include <netinet/in.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>

#pragma pack(1)
struct ConnectionRequest {
  Byte id;
  Long guid;
  Long time;
  Bool security;
};

typedef struct ConnectionRequest ConnectionRequest;

struct ConnectionRequestAccepted {
  Byte id;
  Address client;
  short index;
  Address internal[10];
  Long request_time;
  Long time;
};

typedef struct ConnectionRequestAccepted ConnectionRequestAccepted;

int raknet_connection_request(Server *server, Frame *rframe, Byte *body, DataPacket *packet) {
  ConnectionRequest rdata = *(ConnectionRequest *) body;

  ConnectionRequestAccepted sdata = {
    .id = RAW_ConnectionRequestAccepted,
    .index = 0,
    .request_time = rdata.time,
    .time = get_time_milliseconds() - server->start_time
  };

  sdata.client[0] = 4;
  *(unsigned *) (sdata.client + 1) = packet->address.sin_addr.s_addr;
  *(ushort *) (sdata.client + 5) = htons(packet->address.sin_port);

  Address internal = { 4, 255, 255, 255, 255, 0x4a, 0xbc };

  for (unsigned index = 0; index < 10; ++index) {
    (void) memcpy(sdata.internal + index, &internal, sizeof(internal));
  }

  Frame frame = {
    .id = RAW_FrameSet,
    .sequence_number = { 0, 0, 0 },
    .flags = RAW_UNRELIABLE,
    .length = packet->length * 8
  };

  PacketBuffer buffer = {
    .data = packet->data,
    .pointer = packet->data
  };

  pb_put_frame(&buffer, &frame);
  pb_put_struct(&buffer, &sdata, sizeof(sdata));

  packet->length = buffer.pointer - buffer.data;

  Packet *spacket;
  if (packet_create(packet, &spacket)) return 1;

  spacket->type = RAW_ConnectionRequestAccepted;

  if (pqueue_push(&server->queue, spacket)) return 2;

  return 0;
}
