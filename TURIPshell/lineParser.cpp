#include "lineParser.h"

int getSegment(const char* line, int index, char** segment){
  int numofSegment = 0;
  int lenLine = strlen(line) + 1;
  char* input = new char[lenLine];

  strcpy(input, line);

  // Convert ' ' to '\0' (ignore in "")
  for(size_t i = 0; i < lenLine; i++){
    if(input[i] == '"'){
      i++;
      for(; i < lenLine; i++){
        if(input[i] == '"' && input[i-1] != '\\' ) break;
      }
    }
    if(input[i] == ' ') input[i] = '\0';
  }

  // Count segments.
  for (size_t i = 0; i < lenLine; i++) {
    if(input[i] != '\0'){
      if(numofSegment == index){
        *segment = new char[strlen((char*)(&input[i])) + 1];
        strcpy(*segment, (char*)(&input[i]));
      }
      numofSegment++;
    }
    for(; i < lenLine; i++){
      if (input[i] == '\0') break;
    }
  }

  delete[] input;
  return numofSegment;
}

bool isOptionExist(const char* line, const char* option){
  char* opStr = NULL;
  int numofSegment = 1;
  for (size_t i = 0; i < numofSegment; i++) {
    if(opStr != NULL) delete[] opStr;
    numofSegment = getSegment(line, i, &opStr);
    if(!strcmp(opStr, option)){
      if(opStr != NULL) delete[] opStr;
      return true;
    }
  }
  if(opStr != NULL) delete[] opStr;
  return false;
}

int getArgFromOption(const char* line, const char* option, char** arg){
  char* opStr = NULL;
  int numofSegment = 1;
  for (size_t i = 0; i < numofSegment; i++) {
    if(opStr != NULL) delete[] opStr;
    numofSegment = getSegment(line, i, &opStr);
    if(!strcmp(opStr, option)){
      getSegment(line, i+1, arg);
      if(opStr != NULL) delete[] opStr;
      return i;
    }
  }
  if(opStr != NULL) delete[] opStr;
  return -1;
}
