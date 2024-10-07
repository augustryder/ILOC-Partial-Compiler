#pragma once
#include "types.h"

typedef struct {
    int val;
    int sr;
    int vr;
    int pr;
    int nu;
} Operand;

typedef struct {
    Opcode opcode;
    Operand* op1;
    Operand* op2;
    Operand* op3;
} Inst;

Inst* makeInst(Opcode opcode, Operand op1, Operand op2, Operand op3);
void printInst(Inst* val);
void tPrintInst(Inst* val); //precede with a // so if you print it it still executes
void prettyPrintInst(Inst* val); // add a selector for SR,VR,PR
