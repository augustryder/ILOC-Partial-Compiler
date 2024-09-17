#include "line.h"
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

void printLine(Line* val) {
  printf("(%d, %d)\t", val->x, val->y);
  return;
}

void tPrintLine(Line* val) {
  printf("| %8d | %8d |\n", val->x, val->y);
  return;
}

Line* makeLine(int x, int y) {
  Line *val = (Line*) malloc(sizeof(Line));
  assertCondition(val != NULL, "Memory error allocating line.");
  val->x = x;
  val->y = y;
  return val;
}
