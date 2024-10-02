#pragma once
#include "types.h"

typedef struct {
    int val;
    int sr;
    int vr;
    int pr;
} Operand;

typedef struct {
    Opcode opcode;
    Operand sr1;
    Operand sr2;
    Operand dest;
} Inst;

Inst* makeInst(Opcode opcode, Operand sr1, Operand sr2, Operand dest);
void printInst(Inst* val);
void tPrintInst(Inst* val); //precede with a // so if you print it it still executes
void prettyPrintInst(Inst* val); // add a selector for SR,VR,PR
