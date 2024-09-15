#include "utils.h"
#include <stdlib.h>

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
  while(c != EOF){
    if(!isspace(c)){
      ungetc(c,input);
      break;
    }
     c = getc(input);
  }
}

void skipBlankspace(FILE* input){
  char c = getc(input);
  while(c != EOF){
    if(c != ' ' && c != '\t'){
      ungetc(c,input);
      break;
    }
    c = getc(input);
  }
}

bool readNumber(FILE* input, int* out){
  int retval = 0;
  while(true){
    char c = getc(input);
    if(c > '9' || c < '0'){
      ungetc(c,input);
      break;
    }else{
      retval = retval * 10 + (c - '0');
    }
  }
  *out = retval;
  return true;
}
