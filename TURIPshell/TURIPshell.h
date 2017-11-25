#ifndef TURIP_SHELL_H
#define TURIP_SHELL_H

// #include <stddef.h>
// #include <stdint.h>
// #include <string.h>
#include <Arduino.h>
#include <TURIPcommon.h>
#include "lineParser.h"
#include "urlParser.h"
#include "TURIPserver.h"
#include "TURIPclient.h"

String TURIPshell(const char* line);
String TURIPshell(String line);
String strReadFromPort(const char* port);
String strWriteToPort(const char* port, const char* data);
String strReadAcrossBridge(const char* id, const char* port);
String strWriteAcrossBridge(const char* id, const char* port, const char* data);
uint64_t strToId(const char* id);
TURIPdataType strToType(const char* type);

#endif
