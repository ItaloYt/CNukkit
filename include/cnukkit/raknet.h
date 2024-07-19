#ifndef CNUKKIT_RAKNET_H
#define CNUKKIT_RAKNET_H

#include "cnukkit/server.h"
#include "cnukkit/packet.h"

enum RawPacketType {
  RAW_UnconnectedPing = 0x01,
  RAW_UnconnectedPong = 0x1c,

  RAW_OpenConnectionRequest1 = 0x5,
  RAW_OpenConnectionReply1 = 0x6,

  RAW_OpenConnectionRequest2 = 0x7,
  RAW_OpenConnectionReply2 = 0x8,

  RAW_FrameSet = 0x84,

  RAW_ConnectionRequest = 0x9,
  RAW_ConnectionRequestAccepted = 0x10,

  RAW_IncomingConnection = 0x13,

  RAW_ConnectedPing = 0x0,
  RAW_ConnectedPong = 0x3,

  RAW_ACK = 0xc0,
  RAW_NACK = 0xa0,

  RAW_GamePacket = 0xfe
};

typedef enum RawPacketType RawPacketType;

void raknet_send_packets(Server *server);
int raknet_handle_packets(Server *server, DataPacket *packet);
int raknet_is_recorded(Server *server, const triad sequence);
int raknet_unconnected_ping(Server *server, DataPacket *packet);
int raknet_oc_request1(Server *server, DataPacket *packet);
int raknet_oc_request2(Server *server, DataPacket *packet);

int raknet_connection_request(Server *server, Frame *frame, Byte *body, DataPacket *packet);
int raknet_incoming_connection(Server *server, Frame *frame, Byte *body, DataPacket *packet);
int raknet_connected_ping(Server *server, Frame *frame, Byte *body, DataPacket *packet);
int raknet_ack(Server *server, Frame *frame, Byte *body, DataPacket *packet);
int raknet_nack(Server *server, Frame *frame, Byte *body, DataPacket *packet);
int raknet_game_packet(Server *server, Frame *frame, Byte *body, DataPacket *packet);

#endif
