#include "cnukkit/raknet.h"
#include <stdio.h>
#include <string.h>

triad recovery[RECOVERY_LENGTH];
ushort recovery_index;

static int raw_packets(Server *server, Frame *frame, Byte *body, DataPacket *packet);

int raknet_handle_packets(Server *server, DataPacket *packet) {
  PacketBuffer buffer = {
    .data = packet->data,
    .pointer = packet->data
  };

  Frame frame = {0};

  if (packet->data[0] == RAW_FrameSet) {
    pb_get_frame(&buffer, &frame);

    recovery[recovery_index][0] = frame.sequence_number[0];
    recovery[recovery_index][1] = frame.sequence_number[1];
    recovery[recovery_index][2] = frame.sequence_number[2];

    recovery_index = (recovery_index + 1) % RECOVERY_LENGTH;
  }

  Byte body[frame.length / 8];

  if (frame.length)
    (void) memcpy(body, buffer.pointer, frame.length / 8);

  if (raw_packets(server, &frame, body, packet)) return 1;

  return 0;
}

static int raw_packets(Server *server, Frame *frame, Byte *body, DataPacket *packet) {
  enum RawPacketType type = (packet->data[0] == RAW_FrameSet ? body[0] : packet->data[0]);

  switch(type) {
    case RAW_UnconnectedPing:
    if (raknet_unconnected_ping(server, packet)) return 2;
    break;

    case RAW_OpenConnectionRequest1:
    if (raknet_oc_request1(server, packet)) return 3;
    break;

    case RAW_OpenConnectionRequest2:
    if (raknet_oc_request2(server, packet)) return 4;
    break;

    case RAW_ConnectionRequest:
    if (raknet_connection_request(server, frame, body, packet)) return 5;
    break;

    case RAW_IncomingConnection:
    if (raknet_incoming_connection(server, frame, body, packet)) return 6;
    break;

    case RAW_ACK:
    if (raknet_ack(server, frame, body, packet)) return 7;
    break;

    case RAW_ConnectedPing:
    if (raknet_connected_ping(server, frame, body, packet)) return 8;
    break;

    case RAW_GamePacket:
    if (server_handle_packet(server, frame, body + 1, packet)) return 9;
    break;

    // case RAW_NACK:
    // if (raknet_nack(server, frame, body, packet)) return 9;
    // break;

    default:
    (void) printf("Unhandled raw packet%s: %#hhx\n", (packet->data[0] == RAW_FrameSet ? "(Frame)" : ""), type);
    return 1;
  }

  return 0;
}
