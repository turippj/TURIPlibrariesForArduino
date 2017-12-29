#include "urlParser.h"
#include <string.h>

int getUrlSegment(char* input, int index, char** segment){
  int numofSegment = 0;
  int inputLen = strlen(input) + 1;

  if (*segment != NULL){
    return -1;
  }

  char* _input = new char[inputLen];
  strcpy(_input, input);

  // Convert '/' to '\0'
  for(size_t i = 0; i < inputLen; i++){
    if(input[i] == ' ') input[i] = '\0';
  }

  // Count segments.
  for (size_t i = 0; i < inputLen; i++) {
    if(input[i] != '\0'){
      if(numofSegment == index){
        *segment = new char[strlen((char*)(&input[i])) + 1];
        strcpy(*segment, (char*)(&input[i]));
      }
      numofSegment++;
    }
    for(; i < inputLen; i++){
      if (input[i] == '\0') break;
    }
  }

  delete[] _input;
  return numofSegment;
}
