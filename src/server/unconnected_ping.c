#include "cnukkit/server.h"
#include "cnukkit/raktypes.h"

#include <alloca.h>
#include <stdio.h>
#include <string.h>
#include <sys/endian.h>
#include <sys/socket.h>
#include <errno.h>

#pragma pack(1)
struct UnconnectedPing {
  char id;
  Long time;
  Magic magic;
  Long guid;
};

struct UnconnectedPong {
  Byte id;
  Long time;
  Long guid;
  Magic magic;
  ushort motd_length;
  char motd[];
};

int server_unconnected_ping(
  const struct Server *server,
  struct RawPacket *packet) {

  if (packet->length != sizeof(struct UnconnectedPing)) {
    (void) printf("UnconnectedPing packet is corrupted: %u\n", packet->length);
    return 1;
  }

  const struct UnconnectedPing rdata =
    *(struct UnconnectedPing *) packet->data;

  struct UnconnectedPong *sdata = 
    (struct UnconnectedPong *) packet->data;
  sdata->id = PT_UnconnectedPong;
  sdata->time = rdata.time;
  sdata->guid = GUID;
  sdata->magic[0] = rdata.magic[0];
  sdata->magic[1] = rdata.magic[1];
  sdata->motd_length = htons(server->motd.length);
  
  (void) memcpy(
    sdata->motd, server->motd.data, server->motd.length);

  packet->length = sizeof(*sdata) + server->motd.length;

  (void) printf(
    "Socket: %d\n"
    "Data: %p\n"
    "Length: %u\n"
    "Address: %p\n"
    "Address Size: %u\n",
    server->socket, packet->data,
    packet->length, &packet->address,
    packet->address_size);

  (void) puts("-----------------------");
  (void) printf(
    "ID: %#x\n"
    "Time: %#llx\n"
    "GUID: %#llx\n"
    "Magic: %#llx%llx\n"
    "MOTD Length: %#hx\n"
    "MOTD: %s\n",
    sdata->id, sdata->time,
    sdata->guid, sdata->magic[0],
    sdata->magic[1], sdata->motd_length,
    sdata->motd);

  if(sendto(
    server->socket, packet->data, packet->length,
    0, &packet->address, packet->address_size) == -1) {

    printf("Sendto Error: %s\n", strerror(errno));
    return 4;
  }

  return 0;
}
