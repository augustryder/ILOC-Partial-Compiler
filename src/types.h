#pragma once

typedef enum {
    INST, REG, CONST, COMMA, ARROW
} Category;

typedef enum {
    LOAD, LOADI,
    STORE,
    ADD, SUB, MULT, 
    LSHIFT, RSHIFT,
    OUTPUT,
    NOP
} Opcode;

char* catToString(Category cat);
char* opToString(Opcode op);

