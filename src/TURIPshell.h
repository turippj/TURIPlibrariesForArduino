#ifndef TURIP_SHELL_H
#define TURIP_SHELL_H

// #include <stddef.h>
// #include <stdint.h>
// #include <string.h>
#include <Arduino.h>
#include "TURIPserver.h"
#include "TURIPclient.h"

typedef struct {
  int method;
  int port;
  int depth;
  uint64_t id;
  char* data;
  char** raw;
} turipShellCommand;

typedef struct {
  int statusCode;
  uint64_t id;
  uint8_t port;
  char data[64];
} turipShellResponse;

void TURIPshell(const char* line, char* output, size_t maxLength);
turipShellCommand turipShellCommandParser(const char* line);
turipShellResponse turipShellLocalGet(turipShellCommand* cmd);
turipShellResponse turipShellLocalPost(turipShellCommand* cmd);
turipShellResponse turipShellBridgeGet(turipShellCommand* cmd);
turipShellResponse turipShellBridgePost(turipShellCommand* cmd);
void turipIdIntToStr(uint64_t id, char* output);
uint64_t turipIdStrToInt(const char* id);

#endif
