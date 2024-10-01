#pragma once
#include "../types.h"

typedef struct {
    int val;
} Operand;

typedef struct inst {
    Opcode opcode;
    Operand sr1;
    Operand sr2;
    Operand dest;
} Inst;

void printInst(Inst* val);
void tPrintInst(Inst* val);
Inst* makeInst(Opcode opcode, Operand sr1, Operand sr2, Operand dest);
