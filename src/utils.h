#pragma once

#include <stdbool.h>
#include <stdio.h>

extern int debugLevel;

void error(char* msg);
void assertCondition(bool condition, char* errmsg);
void debug(char* str);
void debug_l(char* str, int level);
void skipWhitespace(FILE* input);
void skipBlankspace(FILE* input);
void skipComment(FILE* file);
void skipCommentsAndWhite(FILE* file);
bool isAlpha(char c);
bool isNumber(char c);
bool isEndOfWord(char c);
bool readNumber(FILE* input, int* out);
char peek(FILE* file);
