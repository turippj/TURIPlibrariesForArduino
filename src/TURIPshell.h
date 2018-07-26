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
  size_t pathLength;
  uint64_t path[4];
  char data[64];
} turipShellCommand;

typedef struct {
  int statusCode;
  uint64_t id;
  uint8_t port;
  char data[64];
} turipShellResponse;

void TURIPshell(const char* line, char* output, size_t maxLength);
void turipShellCommandParser(const char* line, turipShellCommand* parsed);
void turipShellLocalGet(turipShellCommand* cmd, turipShellResponse* response);
void turipShellLocalPost(turipShellCommand* cmd, turipShellResponse* response);
void turipShellBridgeGet(turipShellCommand* cmd, turipShellResponse* response);
void turipShellBridgePost(turipShellCommand* cmd, turipShellResponse* response);
void turipIdIntToStr(uint64_t id, char* output);
uint64_t turipIdStrToInt(const char* id);

#endif
