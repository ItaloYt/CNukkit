#ifndef CNUKKIT_PACKET_H
#define CNUKKIT_PACKET_H

#include "cnukkit/raktypes.h"
#include <netinet/in.h>

#define MAX_PACKET_SIZE 1024


struct DataPacket {
  Byte data[MAX_PACKET_SIZE];

  struct sockaddr_in address;

  unsigned address_size;
  unsigned length;
};

typedef struct DataPacket DataPacket;

struct Packet {
  struct sockaddr address;

  Byte *data;

  unsigned address_size, length;

  ushort type;
};

typedef struct Packet Packet;

struct PacketBuffer {
  Byte *data;
  Byte *pointer;
};

typedef struct PacketBuffer PacketBuffer;

enum ReliabilityFlags {
  UNRELIABLE = 0,
  UNRELIABLE_SEQUENCED = 1,
  RELIABLE = 2,
  RELIABLE_ORDERED = 3,
  RELIABLE_SEQUENCED = 4,

  RAW_UNRELIABLE = 0,
  RAW_UNRELIABLE_SEQUENCED = 32,
  RAW_RELIABLE = 64,
  RAW_RELIABLE_ORDERED = 96,
  RAW_RELIABLE_SEQUENCED = 128,

  RAW_TYPE = 224,
  RAW_SPLIT = 16
};

typedef enum ReliabilityFlags ReliabilityFlags;

struct Frame {
  Byte id;
  triad sequence_number;
  Byte flags;
  ushort length;
  triad reliable_index;
  triad sequenced_index;
  triad ordered_index;
  Byte order_channel;
  int compound_size;
  short compound_id;
  int index;
};

typedef struct Frame Frame;

#pragma pack(push, 1)
struct Record {
  Bool single_sequence;
  triad sequence_number;
  triad start_sequence;
  triad end_sequence;
};

typedef struct Record Record;
#pragma pack(pop)

int packet_create(const DataPacket *receiver, Packet **packet);
void packet_free(const Packet *packet);
void pb_get_struct(PacketBuffer *buffer, void *pointer, unsigned size);
void pb_get_frame(PacketBuffer *buffer, Frame *frame);
void pb_get_record(PacketBuffer *buffer, Record *record);
void pb_get_triad(PacketBuffer *buffer, triad pointer);
int pb_get_string(PacketBuffer *buffer, string *str);
int pb_get_lstring(PacketBuffer *buffer, lstring *array);
Long pb_get_varint(PacketBuffer *buffer);
Byte pb_get_byte(PacketBuffer *buffer);
int pb_get_int(PacketBuffer *buffer);
short pb_get_short(PacketBuffer *buffer);
Long pb_get_long(PacketBuffer *buffer);

void pb_put_struct(PacketBuffer *buffer, const void *value, unsigned size);
void pb_put_frame(PacketBuffer *buffer, const Frame *value);
void pb_put_record(PacketBuffer *buffer, const Record *value);
void pb_put_triad(PacketBuffer *buffer, const triad value);
void pb_put_string(PacketBuffer *buffer, const string *value);
void pb_put_lstring(PacketBuffer *buffer, const lstring *value);
void pb_put_varint(PacketBuffer *buffer, Long value);
void pb_put_byte(PacketBuffer *buffer, Byte value);
void pb_put_int(PacketBuffer *buffer, int value);
void pb_put_short(PacketBuffer *buffer, short value);
void pb_put_long(PacketBuffer *buffer, Long value);
#endif
