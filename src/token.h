#pragma once
#include "types.h"

typedef struct {
    Category category;
    union {
        Opcode opcode;
        int number;
    } value;
} Token;

void printToken(Token tok);

