#include "parse.h"
#include <stdio.h>
#include "block.h"
#include "utils.h"
#include "scanner.h"

Block* parse(FILE* file) {
    Block* IR = emptyBlock();
    skipCommentsAndWhite(file);
    while (peek(file) != EOF) {
        Token tok = nextToken(file);
        if (tok.category != INST) error("Invalid instruction syntax!");
        switch (tok.value.opcode) {
            case LOAD:
                append(IR, getLoad(file));
                break;
            case LOADI:
                append(IR, getLoadI(file));
                break;
            case STORE:
                append(IR, getStore(file));
                break;
            case ADD:
                append(IR, getArith(ADD, file));
                break;
            case SUB:
                append(IR, getArith(SUB, file));
                break;
            case MULT:
                append(IR, getArith(MULT, file));
                break;
            case LSHIFT:
                append(IR, getArith(LSHIFT, file));
                break;
            case RSHIFT:
                append(IR, getArith(RSHIFT, file));
                break;
            case OUTPUT:
                append(IR, getOutput(file));
                break;
            case NOP:
                append(IR, getNop(file));
                break;
            default:
                error("Token contains invalid opcode.");
        }
        skipCommentsAndWhite(file);
    }
    return IR;
}

Inst* getLoad(FILE* file) {
    Token r1 = nextToken(file);
    if (r1.category != REG) error("Invalid load syntax!");

    Token arrow = nextToken(file);
    if (arrow.category != ARROW) error("Invalid load syntax!");

    Token r2 = nextToken(file);
    if (r2.category != REG) error("Invalid load syntax!");

    skipBlankspace(file);
    char end = getc(file);
    if (end != '\n' && (end != '/' && peek(file) != '/')) error("Invalid load syntax!");
    ungetc(end, file);

    Operand op1 = {.val = -1, 
                   .sr = r1.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    Operand op2 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    Operand op3 = {.val = -1, 
                   .sr = r2.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};


    return makeInst(LOAD, op1, op2, op3);
}

Inst* getLoadI(FILE* file) {
    Token cnst = nextToken(file);
    if (cnst.category != CONST) error("Invalid loadI syntax!");

    Token arrow = nextToken(file);
    if (arrow.category != ARROW) error("Invalid loadI syntax!");

    Token r2 = nextToken(file);
    if (r2.category != REG) error("Invalid loadI syntax!");

    skipBlankspace(file);
    char end = getc(file);
    if (end != '\n' && (end != '/' && peek(file) != '/')) error("Invalid loadI syntax!");
    ungetc(end, file);

    Operand op1 = {.val = cnst.value.number, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    Operand op2 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    Operand op3 = {.val = -1, 
                   .sr = r2.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    return makeInst(LOADI, op1, op2, op3);
}

Inst* getStore(FILE* file) {
    Token r1 = nextToken(file);
    if (r1.category != REG) error("Invalid store syntax!");

    Token arrow = nextToken(file);
    if (arrow.category != ARROW) error("Invalid store syntax!");

    Token r2 = nextToken(file);
    if (r2.category != REG) error("Invalid store syntax!");

    skipBlankspace(file);
    char end = getc(file);
    if (end != '\n' && (end != '/' && peek(file) != '/')) error("Invalid store syntax!");
    ungetc(end, file);

    Operand op1 = {.val = -1, 
                   .sr = r1.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    Operand op2 = {.val = -1, 
                   .sr = r2.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    Operand op3 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    return makeInst(STORE, op1, op2, op3);
}

Inst* getArith(Opcode opcode, FILE* file) {
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
    if (end != '\n' && (end != '/' && peek(file) != '/')) error("Invalid arithmetic syntax!");
    ungetc(end, file);

    Operand op1 = {.val = -1, 
                   .sr = r1.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    Operand op2 = {.val = -1, 
                   .sr = r2.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    Operand op3 = {.val = -1, 
                   .sr = r3.value.number, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    return makeInst(opcode, op1, op2, op3);
}

Inst* getOutput(FILE* file) {
    Token cnst = nextToken(file);
    if (cnst.category != CONST) error("Invalid output syntax!");
    
    skipBlankspace(file);
    char end = getc(file);
    if (end != '\n' && (end != '/' && peek(file) != '/')) error("Invalid output syntax!");
    ungetc(end, file);

    Operand op1 = {.val = cnst.value.number, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    Operand op2 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    Operand op3 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    return makeInst(OUTPUT, op1, op2, op3);
}

Inst* getNop(FILE* file) {
    skipBlankspace(file);
    char end = getc(file);
    if (end != '\n' && (end != '/' && peek(file) != '/')) error("Invalid nop syntax!");
    ungetc(end, file);

    Operand op1 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    Operand op2 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    Operand op3 = {.val = -1, 
                   .sr = -1, 
                   .vr = -1, 
                   .pr = -1, 
                   .nu = -1};

    return makeInst(NOP, op1, op2, op3);
}

