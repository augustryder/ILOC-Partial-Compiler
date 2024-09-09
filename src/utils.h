#pragma once
#include <stdbool.h>

extern int debugLevel;

void error(char* msg);
void assertCondition(bool condition, char* errmsg);
void debug(char* str);
void debug_l(char* str, int level);
