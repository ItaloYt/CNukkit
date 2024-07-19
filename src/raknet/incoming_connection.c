#include "cnukkit/raknet.h"

#pragma pack(1)
struct IncomingConnection {
  Byte id;
  Address server;
  Address internal[20];
  Long incoming_timestamp;
  Long raknet_timestamp;
};

typedef struct IncomingConnection IncomingConnection;

int raknet_incoming_connection(Server *server, Frame *frame, Byte *body, DataPacket *packet) {
  IncomingConnection rdata = *(IncomingConnection *) body;

  (void) rdata;

  return 0;
}
