#ifndef CNUKKIT_RAKTYPES_H
#define CNUKKIT_RAKTYPES_H

typedef unsigned char Byte;
typedef unsigned long long Long;
typedef unsigned short ushort;
typedef Byte u24le[3];
typedef Byte Address[7];
typedef Byte Bool;
typedef Long Magic[2];

#define stringl(literal) (string) { \
  .length = sizeof(literal) - 1, \
  .data = literal \
}

typedef struct String {
  ushort length;
  char *data;
} string;

#endif
