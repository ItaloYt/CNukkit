#include "cnukkit/raknet.h"
#include <stdio.h>

int raknet_nack(Server *server, Frame *frame, Byte *body, DataPacket *packet) {
  PacketBuffer buffer = {
    .data = body,
    .pointer = body + 1
  };

  short record_length = pb_get_short(&buffer);

  (void) printf("NACK Record Length: %hd\nPacket Length: %u\n", record_length, packet->length);

  Record record[record_length];

  for (unsigned index = 0; index < record_length; ++index) {
    pb_get_record(&buffer, record + index);

    if (!record[index].single_sequence) {
      (void) printf(
        "Ranged Sequence Records not supported:\n%#hhx %hhx %hhx\n%#hhx %hhx %hhx\n\n",
        record[index].start_sequence[2], record[index].start_sequence[1], record[index].start_sequence[0],
        record[index].end_sequence[2], record[index].end_sequence[1], record[index].end_sequence[0]);

      continue;
    }
    
    if (raknet_is_recorded(server, record[index].sequence_number))
      (void) printf(
        "Sequence %#hhx %hhx %hhx recorded\n",
        record[index].sequence_number[2], record[index].sequence_number[1], record[index].sequence_number[0]);
  }

  return 0;
}
