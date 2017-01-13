#ifndef TURIP_PORTPERMISSION_H
#define TURIP_PORTPERMISSION_H

/*
type TURIPportPermission

TURIPのポートのパーミッション

READONLY: Readのみ可
READWRITE: Read/Write可
*/
typedef enum en_TURIPportPermission{
  READONLY, READWRITE
} TURIPportPermission;

#endif
