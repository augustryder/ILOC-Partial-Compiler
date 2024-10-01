#pragma once
#include "types.h"

typedef struct {
    int val;
} Operand;

typedef struct {
    Opcode opcode;
    Operand sr1;
    Operand sr2;
    Operand dest;
} Inst;

Inst* makeInst(Opcode opcode, Operand sr1, Operand sr2, Operand dest);
void printInst(Inst* val);
void tPrintInst(Inst* val);
