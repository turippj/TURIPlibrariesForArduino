#ifndef TURIP_DIFENES_H
#define TURIP_DIFENES_H

#define TURIP_PORT_MODEL 0x40
#define TURIP_PORT_SERIAL 0x44
#define TURIP_PORT_LOCALADDR 0x48
#define TURIP_PORT_NULL    0xff

#define TURIP_TYPE_UNKNOWN 0x00
#define TURIP_TYPE_BOOL    0x01
#define TURIP_TYPE_UINT8   0x02
#define TURIP_TYPE_INT8    0x03
#define TURIP_TYPE_UINT16  0x04
#define TURIP_TYPE_INT16   0x05
#define TURIP_TYPE_UINT32  0x06
#define TURIP_TYPE_INT32   0x07
#define TURIP_TYPE_UINT64  0x08
#define TURIP_TYPE_INT64   0x09
#define TURIP_TYPE_FLOAT   0x0a
#define TURIP_TYPE_DOUBLE  0x0b
#define TURIP_TYPE_STRING  0x0c
#define TURIP_TYPE_BINARY  0x0d

#define TURIP_PERMISSION_RO 0
#define TURIP_PERMISSION_RW 1

#define TURIP_METHOD_UNKOWN -1
#define TURIP_METHOD_GET 0
#define TURIP_METHOD_POST 1

#define TURIP_OK 0
#define TURIP_ERR -1
#define TURIP_WRONG_PORT_TYPE -2

typedef int TURIPdataType;

#endif
