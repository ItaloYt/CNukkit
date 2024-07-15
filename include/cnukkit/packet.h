#ifndef CNUKKIT_PACKET_H
#define CNUKKIT_PACKET_H

#include <netinet/in.h>

#define MAX_PACKET_SIZE 1024

enum PacketType {
  PT_UnconnectedPing = 0x01,
  PT_UnconnectedPong = 0x1c,

  PT_OpenConnectionRequest1 = 0x5,
  PT_OpenConnectionReply1 = 0x6,
};

struct RawPacket {
  char data[MAX_PACKET_SIZE];

  struct sockaddr address;

  unsigned address_size;
  unsigned length;
};

#endif
