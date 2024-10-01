#include "parse.h"
#include <stdio.h>
#include "list.h"
#include "utils.h"
#include "scanner.h"

List* parse(FILE* file) {
    List* IR = emptyList();
    skipWhitespace(file);
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
        skipWhitespace(file);
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
    if (peek(file) != '\n') error("Invalid load syntax!");

    Operand sr1;
    Operand sr2;
    Operand dest;
    sr1.val = r1.value.number;
    sr2.val = -1;
    dest.val = r2.value.number;

    return makeInst(LOAD, sr1, sr2, dest);
}

Inst* getLoadI(FILE* file) {
    Token cnst = nextToken(file);
    if (cnst.category != CONST) error("Invalid loadI syntax!");
    Token arrow = nextToken(file);
    if (arrow.category != ARROW) error("Invalid loadI syntax!");
    Token r2 = nextToken(file);
    if (r2.category != REG) error("Invalid loadI syntax!");
    skipBlankspace(file);
    if (peek(file) != '\n') error("Invalid loadI syntax!");

    Operand sr1;
    Operand sr2;
    Operand dest;
    sr1.val = cnst.value.number;
    sr2.val = -1;
    dest.val = r2.value.number;

    return makeInst(LOADI, sr1, sr2, dest);
}

Inst* getStore(FILE* file) {
    Token r1 = nextToken(file);
    if (r1.category != REG) error("Invalid store syntax!");
    Token arrow = nextToken(file);
    if (arrow.category != ARROW) error("Invalid store syntax!");
    Token r2 = nextToken(file);
    if (r2.category != REG) error("Invalid store syntax!");
    skipBlankspace(file);
    if (peek(file) != '\n') error("Invalid store syntax!");

    Operand sr1;
    Operand sr2;
    Operand dest;
    sr1.val = r1.value.number;
    sr2.val = r2.value.number;
    dest.val = -1;

    return makeInst(STORE, sr1, sr2, dest);
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
    if (peek(file) != '\n') error("Invalid arithmetic syntax!");

    Operand sr1;
    Operand sr2;
    Operand dest;
    sr1.val = r1.value.number;
    sr2.val = r2.value.number;
    dest.val = r3.value.number;

    return makeInst(opcode, sr1, sr2, dest);
}

Inst* getOutput(FILE* file) {
    Token cnst = nextToken(file);
    if (cnst.category != CONST) error("Invalid output syntax!");
    skipBlankspace(file);
    if (peek(file) != '\n') error("Invalid output syntax!");

    Operand sr1;
    Operand sr2;
    Operand dest;
    sr1.val = cnst.value.number;
    sr2.val = -1;
    dest.val = -1;

    return makeInst(OUTPUT, sr1, sr2, dest);
}

Inst* getNop(FILE* file) {
    skipBlankspace(file);
    if (peek(file) != '\n') error("Invalid nop syntax!");

    Operand sr1;
    Operand sr2;
    Operand dest;
    sr1.val = -1;
    sr2.val = -1;
    dest.val = -1;

    return makeInst(NOP, sr1, sr2, dest);
}
