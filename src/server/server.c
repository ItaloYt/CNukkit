#include "cnukkit/server.h"

#include <alloca.h>
#include <linux/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#pragma weak GUID
const Long GUID = 0xdeadbeeffeebdaed;

#pragma weak ADDRESS
const unsigned ADDRESS = INADDR_ANY;

#pragma weak PORT
const ushort PORT = 19132;

int server_start(struct Server *server) {
  server->socket = socket(AF_INET, SOCK_DGRAM, 0);

  if (!server->socket) return 1;

  int flags = fcntl(server->socket, F_GETFL, 0);

  if (flags < 0) return 2;

  flags |= O_NONBLOCK;

  if (fcntl(server->socket, F_SETFL, flags) < 0)
    return 3;

  if (setsockopt(
    server->socket, SOL_SOCKET,
    SO_REUSEADDR | SO_REUSEPORT,
    &(int) { 1 }, sizeof(int))) return 4;

  server->address = (struct sockaddr_in){
    .sin_addr.s_addr = ADDRESS,
    .sin_port = htons(PORT),
    .sin_family = AF_INET
  };

  if (bind(
    server->socket, (struct sockaddr *) &server->address,
    sizeof(server->address)) < 0) return 5;

  server_create_motd(server);

  return 0;
}

int server_close(const struct Server *server) {
  if (server->motd.data) free(server->motd.data);
  if (close(server->socket)) return 1;

  return 0;
}
