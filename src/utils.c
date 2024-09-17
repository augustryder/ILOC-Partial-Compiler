#include "utils.h"
#include <stdlib.h>
#include <ctype.h>

int debugLevel = 0; 

void error(char* msg){
  fprintf(stderr, "Error: %s\n", msg);
  exit(EXIT_FAILURE);
}

void assertCondition(bool condition, char* errmsg){
  if (!condition) {
    fprintf(stderr, "Assertion failed: %s\n", errmsg);
    exit(EXIT_FAILURE);
  }
}

void debug(char* str) {
  debug_l(str, 1);
}

void debug_l(char* str, int level) {
  if (debugLevel >= level)
    printf("//%s\n", str);
}

void skipWhitespace(FILE* input){
  char c = getc(input);
  while (c != EOF) {
    if (!isspace(c) && c != '\n') {
      ungetc(c, input);
      break;
    }
    c = getc(input);
  }
}

void skipBlankspace(FILE* input){
  char c = getc(input);
  while (c != EOF) {
    if (c != ' ' && c != '\t') {
      ungetc(c, input);
      break;
    }
    c = getc(input);
  }
}

bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isNumber(char c) {
  return c >= '0' && c <= '9';
}

bool readNumber(FILE* input, int* out){
  int retval = 0;
  while(true){
    char c = getc(input);
    if(!isNumber(c)){
      ungetc(c,input);
      break;
    }else{
      retval = retval * 10 + (c - '0');
    }
  }
  *out = retval;
  return true;
}

char peek(FILE* file) {
  char c = getc(file);
  ungetc(c, file);
  return c;
}
