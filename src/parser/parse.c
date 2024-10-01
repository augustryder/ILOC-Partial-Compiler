#include "parse.h"
#include <stdio.h>
#include "list.h"
#include "../utils.h"
#include "lexer/scanner.h"

List* parse(FILE* file) {
    List* IR = emptyList();

    skipWhitespace(file);
    while (peek(file) != EOF) {
        Token tok = nextToken(file);
        if (tok.category != INST) error("Invalid syntax!");
        switch (tok.value.opcode) {
            case LOAD:
                Inst* inst = getLoad(file);
                append(IR, inst);
                break;
            case LOADI:
                break;
            case STORE:
                break;
            case ADD:
                break;
            case SUB:
                break;
            case MULT:
                break;
            case LSHIFT:
                break;
            case RSHIFT:
                break;
            case OUTPUT:
                break;
            case NOP:
                break;
            default:
                error("Token contains invalid opcode.");
        }
        skipWhitespace(file);
    }
}

Inst* getLoad(FILE* file) {
    Token r1 = nextToken(file);
    if (r1.category != REG) error("Invalid syntax!");
    Token arrow = nextToken(file);
    if (arrow.category != ARROW) error("Invalid syntax!");
    Token r2 = nextToken(file);
    if (r2.category != REG) error("Invalid syntax!");
    skipBlankspace(file);
    if (peek(file) != '\n') error("Invalid syntax!");

    Operand sr1;
    sr1.val = r1.value.number;
    Operand sr2;
    sr2.val = r2.value.number;
    Operand dest;
    dest.val = -1;
    return makeInst(LOAD, sr1, sr2, dest);
}
