#include "cnukkit/packet.h"
#include "cnukkit/server.h"
#include <alloca.h>
#include <stdlib.h>
#include <stdio.h>

#define TAB "    "

#pragma pack(1)
struct BehaviourPack {
  lstring id;
  lstring version;
  Long size;
  lstring key;
  lstring subname;
  lstring identity;
  Bool scripts;
};

typedef struct BehaviourPack BehaviourPack;

struct BehaviourPacks {
  ushort size;
  BehaviourPack *pack;
};

typedef struct BehaviourPacks BehaviourPacks;

struct ResourcePack {
  lstring id;
  lstring version;
  Long size;
  lstring key;
  lstring subname;
  lstring identity;
  Bool scripts;
  Bool ray_tracing;
};

typedef struct ResourcePack ResourcePack;

struct ResourcePacks {
  ushort size;
  ResourcePack *pack;
};

typedef struct ResourcePacks ResourcePacks;

struct URL {
  lstring first;
  lstring second;
};

typedef struct URL URL;

struct URLs {
  Long length;
  struct URL *url;
};

typedef struct URLs URLs;

struct ResourcePacksInfo {
  Byte id;
  Bool required;
  Bool addon_packs;
  Bool scripts;
  Bool force_server_packs;
  BehaviourPacks behaviour;
  ResourcePacks resource;
  URLs urls;
};

typedef struct ResourcePacksInfo ResourcePacksInfo;

static int decode(PacketBuffer *buffer, ResourcePacksInfo *info);
static int decode_behaviour(PacketBuffer *buffer, BehaviourPack *pack);
static int decode_resource(PacketBuffer *buffer, ResourcePack *pack);

int server_resource_pack_info(Server *server, Frame *frame, Byte *body, DataPacket *packet) {
  (void) printf("Packet Length: %u\n", packet->length);

  PacketBuffer buffer = {
    .data = body,
    .pointer = body
  };

  ResourcePacksInfo *pack = alloca(frame->length / 8 - 1);

  if (decode(&buffer, pack)) return 1;

  return 0;
}

static int decode(PacketBuffer *buffer, ResourcePacksInfo *info) {
  info->id = pb_get_byte(buffer);
  info->required = pb_get_byte(buffer);
  info->addon_packs = pb_get_byte(buffer);
  info->force_server_packs = pb_get_byte(buffer);
  info->behaviour.size = pb_get_short(buffer);

  (void) printf(
    "ResourcePackInfo Decode: %#hhx\n"
    "Required: %u\n"
    "Has addon packs: %u\n"
    "Has scripts: %u\n"
    "Force Server Packs: %u\n"
    "BehaviourPack Count: %hu\n",
    info->id, info->required, info->addon_packs,
    info->scripts, info->force_server_packs, info->behaviour.size);

  info->behaviour.pack = malloc(info->behaviour.size * sizeof(BehaviourPack));

  if (!info->behaviour.pack) return 1;

  for (unsigned index = 0; index < info->behaviour.size; ++index) {
    if (decode_behaviour(buffer, info->behaviour.pack + index)) return 2;

    BehaviourPack *pack = info->behaviour.pack + index;

    (void) printf(
      TAB"BehaviourPack %u:\n"
      TAB"ID: %llu %s\n"
      TAB"Version: %llu %s\n"
      TAB"Size: %#llx\n"
      TAB"Content Key: %llu %s\n"
      TAB"Sub Pack Name: %llu %s\n"
      TAB"Content Identity: %llu %s\n"
      TAB"Has Scripts: %u\n",
      index, pack->id.length, pack->id.data,
      pack->version.length, pack->version.data,
      pack->size, pack->key.length, pack->key.data,
      pack->subname.length, pack->subname.data,
      pack->identity.length, pack->identity.data,
      pack->scripts);
  }

  info->resource.size = pb_get_short(buffer);
  info->resource.pack = malloc(info->resource.size * sizeof(ResourcePack));

  if (!info->resource.pack) return 3;

  (void) printf("ResourcePack Count: %hu\n", info->resource.size);

  for (unsigned index = 0; index < info->resource.size; ++index) {
    if (decode_resource(buffer, info->resource.pack + index)) return 4;
    
    ResourcePack *pack = info->resource.pack + index;

    (void) printf(
      TAB"BehaviourPack %u:\n"
      TAB"ID: %llu %s\n"
      TAB"Version: %llu %s\n"
      TAB"Size: %#llx\n"
      TAB"Content Key: %llu %s\n"
      TAB"Sub Pack Name: %llu %s\n"
      TAB"Content Identity: %llu %s\n"
      TAB"Has Scripts: %u\n"
      TAB"Ray Tracing Enabled: %u\n",
      index, pack->id.length, pack->id.data,
      pack->version.length, pack->version.data,
      pack->size, pack->key.length, pack->key.data,
      pack->subname.length, pack->subname.data,
      pack->identity.length, pack->identity.data,
      pack->scripts, pack->ray_tracing);
  }

  info->urls.length = pb_get_varint(buffer);

  (void) printf("URLs Count: %llu\n", info->urls.length);
  
  for (unsigned index = 0; index < info->urls.length; ++index) {
    if (pb_get_lstring(buffer, &info->urls.url[index].first)) return 5;
    if (pb_get_lstring(buffer, &info->urls.url[index].second)) return 6;

    URL *url = info->urls.url + index;

    (void) printf(
      TAB"URL %u:\n"
      TAB"First: %llu %s\n"
      TAB"Second: %llu %s\n",
      index, url->first.length, url->first.data,
      url->second.length, url->second.data);
  }

  return 0;
}

static int decode_behaviour(PacketBuffer *buffer, BehaviourPack *pack) {
  if (pb_get_lstring(buffer, &pack->id)) return 1;
  if (pb_get_lstring(buffer, &pack->version)) return 2;
  pack->size = pb_get_long(buffer);
  if (pb_get_lstring(buffer, &pack->key)) return 3;
  if (pb_get_lstring(buffer, &pack->subname)) return 4;
  pack->scripts = pb_get_byte(buffer);

  return 0;
}

static int decode_resource(PacketBuffer *buffer, ResourcePack *pack) {
  if (pb_get_lstring(buffer, &pack->id)) return 1;
  if (pb_get_lstring(buffer, &pack->version)) return 2;
  pack->size = pb_get_long(buffer);
  if (pb_get_lstring(buffer, &pack->key)) return 3;
  if (pb_get_lstring(buffer, &pack->subname)) return 4;
  pack->scripts = pb_get_byte(buffer);
  pack->ray_tracing = pb_get_byte(buffer);

  return 0;
}
