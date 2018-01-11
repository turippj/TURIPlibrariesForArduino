#ifndef LINE_PARSER_H
#define LINE_PARSER_H

#include <Arduino.h>

int getSegment(const char* line, int index, char** segment);
bool isOptionExist(const char* line, const char* option);
int getArgFromOption(const char* line, const char* option, char** arg);

#endif
