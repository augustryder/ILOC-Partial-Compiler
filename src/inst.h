#pragma once
#include "types.h"

typedef struct {
    int val;
    int sr; // source register
    int vr; // virtual register
    int pr; // physical register
    int nu; // index of vrs next use
    int ns; // index of next store inst
} Operand;

typedef struct {
    Opcode opcode;
    Operand op1;
    Operand op2;
    Operand op3;
    int index;
} Inst;

Inst* makeInst(Opcode opcode, Operand op1, Operand op2, Operand op3, int index);
void printInst(Inst* val);
void tPrintInst(Inst* val);
void prettyPrintInst(Inst* val, int printLevel); 
void printOperand(Operand op);
