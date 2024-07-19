#include "cnukkit/packet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int packet_create(const DataPacket *receiver, Packet **packet) {
  *packet = malloc(sizeof(**packet));
  
  if (!*packet) return 1;

  packet[0]->data = malloc(receiver->length);
  
  if (!packet[0]->data) return 1;

  packet[0]->length = receiver->length;
  packet[0]->address = *(struct sockaddr *) &receiver->address;
  packet[0]->address_size = receiver->address_size;

  (void) memcpy(packet[0]->data, receiver->data, packet[0]->length);

  return 0;
}

void packet_free(const Packet *packet) {
  free(packet->data);
  free((void *) packet);
}

void pb_put_struct(PacketBuffer *buffer, const void *value, unsigned size) {
  (void) memcpy(buffer->pointer, value, size);
  buffer->pointer += size;
}

void pb_get_struct(PacketBuffer *buffer, void *pointer, unsigned size) {
  (void) memcpy(pointer, buffer->pointer, size);
  buffer->pointer += size;
}

void pb_put_frame(PacketBuffer *buffer, const struct Frame *value) {
  pb_put_byte(buffer, value->id);
  pb_put_triad(buffer, value->sequence_number);
  pb_put_byte(buffer, value->flags);
  pb_put_short(buffer, value->length);

  switch(value->flags & RAW_TYPE) {
    case RAW_UNRELIABLE_SEQUENCED:
    pb_put_triad(buffer, value->sequenced_index);
    pb_put_triad(buffer, value->ordered_index);
    pb_put_byte(buffer, value->order_channel);
    break;

    case RAW_RELIABLE:
    pb_put_triad(buffer, value->reliable_index);
    break;

    case RAW_RELIABLE_ORDERED:
    pb_put_triad(buffer, value->reliable_index);
    pb_put_triad(buffer, value->ordered_index);
    pb_put_byte(buffer, value->order_channel);
    break;

    case RAW_RELIABLE_SEQUENCED:
    pb_put_triad(buffer, value->reliable_index);
    pb_put_triad(buffer, value->sequenced_index);
    pb_put_triad(buffer, value->ordered_index);
    pb_put_byte(buffer, value->order_channel);
    break;

    default: break;
  };

  if (value->flags & RAW_SPLIT) {
    pb_put_int(buffer, value->compound_size);
    pb_put_short(buffer, value->compound_id);
    pb_put_int(buffer, value->index);
  }
}

void pb_get_frame(PacketBuffer *buffer, struct Frame *frame) {
  frame->id = pb_get_byte(buffer);
  pb_get_triad(buffer, frame->sequence_number);
  frame->flags = pb_get_byte(buffer);
  frame->length = pb_get_short(buffer);

  switch(frame->flags & RAW_TYPE) {
    case RAW_UNRELIABLE_SEQUENCED:
    pb_get_triad(buffer, frame->sequenced_index);
    pb_get_triad(buffer, frame->ordered_index);
    frame->order_channel = pb_get_byte(buffer);
    break;

    case RAW_RELIABLE:
    pb_get_triad(buffer, frame->reliable_index);
    break;

    case RAW_RELIABLE_ORDERED:
    pb_get_triad(buffer, frame->reliable_index);
    pb_get_triad(buffer, frame->ordered_index);
    frame->order_channel = pb_get_byte(buffer);
    break;

    case RAW_RELIABLE_SEQUENCED:
    pb_get_triad(buffer, frame->reliable_index);
    pb_get_triad(buffer, frame->sequenced_index);
    pb_get_triad(buffer, frame->ordered_index);
    frame->order_channel = pb_get_byte(buffer);
    break;

    default: break;
  };

  if (frame->flags & RAW_SPLIT) {
    frame->compound_size = pb_get_int(buffer);
    frame->compound_id = pb_get_short(buffer);
    frame->index = pb_get_int(buffer);
  }
}

void pb_put_record(PacketBuffer *buffer, const struct Record *value) {
  pb_put_byte(buffer, value->single_sequence);

  if (value->single_sequence) {
    pb_put_triad(buffer, value->sequence_number);

    return;
  }

  pb_put_triad(buffer, value->start_sequence);
  pb_put_triad(buffer, value->end_sequence);
}

void pb_get_record(PacketBuffer *buffer, struct Record *record) {
  record->single_sequence = pb_get_byte(buffer);

  if (record->single_sequence) {
    pb_get_triad(buffer, record->sequence_number);

    return;
  }
  
  pb_get_triad(buffer, record->start_sequence);
  pb_get_triad(buffer, record->end_sequence);
}

void pb_put_triad(PacketBuffer *buffer, const triad value) {
  buffer->pointer[0] = value[0];
  buffer->pointer[1] = value[1];
  buffer->pointer[2] = value[2];

  buffer->pointer += sizeof(triad);
}

void pb_get_triad(PacketBuffer *buffer, triad pointer) {
  pointer[0] = buffer->pointer[0];
  pointer[1] = buffer->pointer[1];
  pointer[2] = buffer->pointer[2];

  buffer->pointer += sizeof(triad);
}

void pb_put_string(PacketBuffer *buffer, const string *value) {
  // adapt
  pb_put_short(buffer, value->length);
  pb_put_struct(buffer, value->data, value->length);
}

int pb_get_string(PacketBuffer *buffer, string *str) {
  str->length = pb_get_short(buffer);
  str->data = malloc(str->length + 1);

  if (!str->data) return 1;

  pb_get_struct(buffer, str->data, str->length);

  return 0;
}

void pb_put_lstring(PacketBuffer *buffer, const lstring *value) {
  pb_put_varint(buffer, value->length);
  pb_put_struct(buffer, value->data, value->length);
}

int pb_get_lstring(PacketBuffer *buffer, lstring *array) {
  array->length = pb_get_varint(buffer);
  array->data = malloc(array->length);

  if (!array->data) return 1;

  pb_get_struct(buffer, array->data, array->length);

  return 0;
}

void pb_put_varint(PacketBuffer *buffer, Long value) {
  varint number = {
    .size = 0
  };

  while (value >= 0x80) {
    number.value[number.size++] = (value & 0x7f) | 0x80;
    value >>= 7;
  }

  number.value[number.size++] = value;

  pb_put_struct(buffer, number.value, number.size);
}

Long pb_get_varint(PacketBuffer *buffer) {
  Byte size = 1;
  Long value = 0;

  for (; size <= 10; ++size) {
    Byte byte = pb_get_byte(buffer);
    value |= (byte & 0x7f) << (7 * (size - 1));

    if (!(byte & 0x80)) break;
  }

  if (size > 8)
    (void) printf("GET VARINT OVERFLOWING\n");

  return value;
}

void pb_put_byte(PacketBuffer *buffer, Byte value) {
  buffer->pointer[0] = value;

  buffer->pointer += sizeof(Byte);
}

Byte pb_get_byte(PacketBuffer *buffer) {
  Byte value = buffer->pointer[0];

  buffer->pointer += sizeof(Byte);

  return value;
}

void pb_put_int(PacketBuffer *buffer, int value) {
  *(int *) buffer->pointer = swap(value);

  buffer->pointer += sizeof(int);
}

int pb_get_int(PacketBuffer *buffer) {
  // TODO: Change this to work with host endianess
  int value = *(int *) buffer->pointer;
  value = swap(value);

  buffer->pointer += sizeof(int);

  return value;
}

void pb_put_short(PacketBuffer *buffer, short value) {
  *(short *) buffer->pointer = htons(value);

  buffer->pointer += sizeof(short);
}

short pb_get_short(PacketBuffer *buffer) {
  short value = ntohs(*(short *) buffer->pointer);

  buffer->pointer += sizeof(short);

  return value;
}

void pb_put_long(PacketBuffer *buffer, Long value) {
  *(Long *) buffer->pointer = htonl(value);

  buffer->pointer += sizeof(Long);
}

Long pb_get_long(PacketBuffer *buffer) {
  Long value = ntohl(*(Long *) buffer->pointer);

  buffer->pointer += sizeof(Long);

  return value;
}
