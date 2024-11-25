#include "parse.h"
#include <stdio.h>
#include "block.h"
#include "utils.h"
#include "scanner.h"

Block* parse(FILE* file) {
    Block* IR = emptyBlock();
    int index = 0;
    skipCommentsAndWhite(file);
    while (peek(file) != EOF) {
        Token tok = nextToken(file);
        if (tok.category != INST) error("Invalid instruction syntax!");
        switch (tok.value.opcode) {
            case LOAD:
                append(IR, getLoad(file, index++));
                break;
            case LOADI:
                append(IR, getLoadI(file, index++));
                break;
            case STORE:
                append(IR, getStore(file, index++));
                break;
            case ADD:
                append(IR, getArith(ADD, file, index++));
                break;
            case SUB:
                append(IR, getArith(SUB, file, index++));
                break;
            case MULT:
                append(IR, getArith(MULT, file, index++));
                break;
            case LSHIFT:
                append(IR, getArith(LSHIFT, file, index++));
                break;
            case RSHIFT:
                append(IR, getArith(RSHIFT, file, index++));
                break;
            case OUTPUT:
                append(IR, getOutput(file, index++));
                break;
            case NOP:
                append(IR, getNop(file, index++));
                break;
            default:
                error("Token contains invalid opcode.");
        }
        skipCommentsAndWhite(file);
    }
    return IR;
}

Inst* getLoad(FILE* file, int index) {
    Token r1 = nextToken(file);
    if (r1.category != REG) error("Invalid load syntax!");

    Token arrow = nextToken(file);
    if (arrow.category != ARROW) error("Invalid load syntax!");

    Token r2 = nextToken(file);
    if (r2.category != REG) error("Invalid load syntax!");

    skipBlankspace(file);
    char end = getc(file);
    if (!isEndOfWord(end) && (end != '/' && peek(file) != '/')) error("Invalid load syntax!");
    ungetc(end, file);

    Operand op1 = {.val = -1, 
                   .sr = r1.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    Operand op2 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    Operand op3 = {.val = -1, 
                   .sr = r2.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};


    return makeInst(LOAD, op1, op2, op3, index);
}

Inst* getLoadI(FILE* file, int index) {
    Token cnst = nextToken(file);
    if (cnst.category != CONST) error("Invalid loadI syntax!");

    Token arrow = nextToken(file);
    if (arrow.category != ARROW) error("Invalid loadI syntax!");

    Token r2 = nextToken(file);
    if (r2.category != REG) error("Invalid loadI syntax!");

    skipBlankspace(file);
    char end = getc(file);
    if (!isEndOfWord(end) && (end != '/' && peek(file) != '/')) error("Invalid loadI syntax!");
    ungetc(end, file);

    Operand op1 = {.val = cnst.value.number, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    Operand op2 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    Operand op3 = {.val = -1, 
                   .sr = r2.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    return makeInst(LOADI, op1, op2, op3, index);
}

Inst* getStore(FILE* file, int index) {
    Token r1 = nextToken(file);
    if (r1.category != REG) error("Invalid store syntax!");

    Token arrow = nextToken(file);
    if (arrow.category != ARROW) error("Invalid store syntax!");

    Token r2 = nextToken(file);
    if (r2.category != REG) error("Invalid store syntax!");

    skipBlankspace(file);
    char end = getc(file);
    if (!isEndOfWord(end) && (end != '/' && peek(file) != '/')) error("Invalid store syntax!");
    ungetc(end, file);

    Operand op1 = {.val = -1, 
                   .sr = r1.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    Operand op2 = {.val = -1, 
                   .sr = r2.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    Operand op3 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    return makeInst(STORE, op1, op2, op3, index);
}

Inst* getArith(Opcode opcode, FILE* file, int index) {
    Token r1 = nextToken(file);
    if (r1.category != REG) error("Invalid arithmetic syntax!");

    Token comma = nextToken(file);
    if (comma.category != COMMA) error("Invalid arithmetic syntax!");

    Token r2 = nextToken(file);
    if (r2.category != REG) error("Invalid arithmetic syntax!");

    Token arrow = nextToken(file);
    if (arrow.category != ARROW) error("Invalid arithmetic syntax!");

    Token r3 = nextToken(file);
    if (r3.category != REG) error("Invalid arithmetic syntax!");

    skipBlankspace(file);
    char end = getc(file);
    if (!isEndOfWord(end) && (end != '/' && peek(file) != '/')) error("Invalid arithmetic syntax!");
    ungetc(end, file);

    Operand op1 = {.val = -1, 
                   .sr = r1.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    Operand op2 = {.val = -1, 
                   .sr = r2.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    Operand op3 = {.val = -1, 
                   .sr = r3.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    return makeInst(opcode, op1, op2, op3, index);
}

Inst* getOutput(FILE* file, int index) {
    Token cnst = nextToken(file);
    if (cnst.category != CONST) error("Invalid output syntax!");
    
    skipBlankspace(file);
    char end = getc(file);
    if (!isEndOfWord(end) && (end != '/' && peek(file) != '/')) error("Invalid output syntax!");
    ungetc(end, file);

    Operand op1 = {.val = cnst.value.number, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    Operand op2 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    Operand op3 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    return makeInst(OUTPUT, op1, op2, op3, index);
}

Inst* getNop(FILE* file, int index) {
    skipBlankspace(file);
    char end = getc(file);
    if (!isEndOfWord(end) && (end != '/' && peek(file) != '/')) error("Invalid nop syntax!");
    ungetc(end, file);

    Operand op1 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    Operand op2 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    Operand op3 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1,
                   .ns = -1};

    return makeInst(NOP, op1, op2, op3, index);
}
