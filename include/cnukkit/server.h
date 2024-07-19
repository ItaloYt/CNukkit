#ifndef CNUKKIT_SERVER_H
#define CNUKKIT_SERVER_H

#include "cnukkit/packet_queue.h"
#include "cnukkit/raktypes.h"

enum GamePacketType {
  GAME_ResourcePackInfo = 6,
};

struct Server {
  struct sockaddr_in address;

  PacketQueue queue;

  string title, subtitle;
  string version;

  string motd;

  Long start_time;

  unsigned protocol;
  unsigned player_count;
  unsigned player_maximum;

  unsigned mtu;

  int socket;
};

typedef struct Server Server;

#define RECOVERY_LENGTH 10

extern triad recovery[RECOVERY_LENGTH];
extern ushort recovery_index;

extern const Long GUID;
extern const unsigned ADDRESS;
extern const ushort PORT;

int server_start(Server *server);
int server_loop(Server *server);
void server_create_motd(Server *server);
int server_close(Server *server);

int server_handle_packet(Server *server, Frame *frame, Byte *body, DataPacket *packet);

int server_resource_pack_info(Server *server, Frame *frame, Byte *body, DataPacket *packet);

#endif
