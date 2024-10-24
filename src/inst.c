#include "inst.h"
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "types.h"

void printOperand(Operand op) {
  printf("(%d, %d, %d, %d, %d) ", op.val, op.sr, op.vr, op.pr, op.nu);
  return;
}

void printInst(Inst* val) {
  printf("%s ", opToString(val->opcode));
  printOperand(val->op1);
  printOperand(val->op2);
  printOperand(val->op3);
  printf("\n");
  return;
}

void tPrintInst(Inst* val) {
  printf("| %-8s | %-8d | %-8d | %-8d |\n", opToString(val->opcode), val->op1.val, val->op2.val, val->op3.val);
  return;
}

void prettyPrintInst(Inst* val) {
  printf("%-8s", opToString(val->opcode));

  if (val->op1.val == -1) {
    printf("\n");
    return; 
  }

  if (val->opcode == LOADI || val->opcode == OUTPUT) printf("%-8d", val->op1.val);
  else printf("r%-7d", val->op1.val);

  if (val->op2.val == -1 || val->opcode == STORE) printf("%-8s", "");
  else printf(", r%-5d", val->op2.val);

  if (val->op3.val == -1) {
    if (val->opcode == STORE) printf("=> r%-4d", val->op2.val);
    printf("\n");
    return; 
  }
  else printf("=> r%-4d", val->op3.val);
  printf("\n");
  return;
}

Inst* makeInst(Opcode opcode, Operand op1, Operand op2, Operand op3) {
  Inst* val = (Inst*) malloc(sizeof(Inst));
  assertCondition(val != NULL, "Memory error allocating Inst.");
  val->opcode = opcode;
  val->op1 = op1;
  val->op2 = op2;
  val->op3 = op3;
  return val;
}
