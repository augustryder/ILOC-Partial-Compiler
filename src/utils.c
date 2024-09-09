#include "utils.h"
#include <stdio.h>
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
