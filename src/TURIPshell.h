#ifndef TURIP_SHELL_H
#define TURIP_SHELL_H

// #include <stddef.h>
// #include <stdint.h>
// #include <string.h>
#include <Arduino.h>
#include "TURIPserver.h"
#include "TURIPclient.h"

typedef struct {
  uint8_t method;
  uint8_t port;
  uint64_t path[4];
  uint8_t path_depth;
  char data[64];
} turipShellCommand;

typedef struct {
  int statusCode;
  uint64_t id;
  uint8_t port;
  char data[64];
} turipShellResponse;

void TURIPshell(const char* line, char* response, size_t response_maxlength);
turipShellCommand turipShellCommandParser(const char* line);
turipShellResponse turipShellLocalGet(turipShellCommand* cmd);
turipShellResponse turipShellLocalPost(turipShellCommand* cmd);
turipShellResponse turipShellBridgeGet(turipShellCommand* cmd);
turipShellResponse turipShellBridgePost(turipShellCommand* cmd);
void turipId_itoa(uint64_t id, char* response, size_t response_maxlength);
uint64_t turipId_atoi(const char* id);

#endif
