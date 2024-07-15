#include "cnukkit/server.h"
#include <stdio.h>
#include <stdlib.h>

static void inline create(struct Server *server, unsigned length) {
  server->motd.length = snprintf(
    server->motd.data, length,
    "MCPE;%s;%u;%s;%u;%u;%llu;%s;Survival;1;19132;19133;",
    server->title.data, server->protocol,
    server->version.data, server->player_count,
    server->player_maximum, GUID, server->subtitle.data);
}

void server_create_motd(struct Server *server) {
  create(server, 0);
  server->motd.data = malloc(server->motd.length);
  create(server, server->motd.length);
}
