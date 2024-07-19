#include "cnukkit/server.h"
#include "cnukkit/packet.h"
#include "cnukkit/raknet.h"

#include <sys/socket.h>
#include <netinet/in.h>

struct DataPacket receiver;

int server_loop(Server *server) {
  while (1) {
    receiver.length = recvfrom(
      server->socket, receiver.data, MAX_PACKET_SIZE, MSG_DONTWAIT, 
      (struct sockaddr *) &receiver.address, &receiver.address_size);

    if (receiver.length == -1 || !receiver.address.sin_addr.s_addr || !receiver.address.sin_port)
      continue;

    if (raknet_handle_packets(server, &receiver)) return 1;

    raknet_send_packets(server);
  }

  return 0;
}
