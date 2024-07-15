#ifndef CNUKKIT_SERVER_H
#define CNUKKIT_SERVER_H

#include "cnukkit/packet.h"
#include "cnukkit/raktypes.h"

struct Server {
  struct sockaddr_in address;

  string title, subtitle;
  string version;

  string motd;

  unsigned protocol;
  unsigned player_count;
  unsigned player_maximum;

  int socket;
};

extern const Long GUID;
extern const unsigned ADDRESS;
extern const ushort PORT;

int server_start(struct Server *server);
int server_packets(const struct Server *server);
int server_unconnected_ping(
  const struct Server *server, 
  struct RawPacket *packet);
void server_create_motd(struct Server *server);
int server_close(const struct Server *server);

#endif
