#ifndef CNUKKIT_RAKTYPES_H
#define CNUKKIT_RAKTYPES_H

typedef unsigned char Byte;
typedef unsigned long long Long;
typedef unsigned short ushort;
typedef Byte triad[3];
typedef Byte Address[7];
typedef Byte Bool;
typedef Long Magic[2];

struct Varint {
  Byte value[10];

  Byte size;
};

typedef struct Varint varint;

#define swap(value)\
((value & 0xff) << 24) | ((value & 0xff00) << 8) | \
((value & 0xff0000) >> 8) | ((value & 0xff000000) >> 24)

#define stringl(literal) (string) { \
  .length = sizeof(literal) - 1, \
  .data = literal \
}

struct String {
  char *data;

  ushort length;
};

typedef struct String string;

struct LString {
  char *data;

  Long length;
};

typedef struct LString lstring;

#endif
