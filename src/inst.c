#include "inst.h"
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "types.h"

void printOperand(Operand op) {
  printf("(%d, %d, %d, %d, %d, %d) ", op.val, op.sr, op.vr, op.pr, op.nu, op.ns);
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
  printf("| %-6d | %-6s ", val->index, opToString(val->opcode));
  printf("| %-6d | %-6d | %-6d | %-6d | %-6d | %-6d |", val->op1.val, val->op1.sr, val->op1.vr, val->op1.pr, val->op1.nu, val->op1.ns);
  printf("| %-6d | %-6d | %-6d | %-6d | %-6d | %-6d |", val->op2.val, val->op2.sr, val->op2.vr, val->op2.pr, val->op2.nu, val->op2.ns);
  printf("| %-6d | %-6d | %-6d | %-6d | %-6d | %-6d |\n", val->op3.val, val->op3.sr, val->op3.vr, val->op3.pr, val->op3.nu, val->op3.ns);
  return;
}

void prettyPrintInst(Inst* val, int printLevel) {
  printf("%-8s", opToString(val->opcode));

  int op1, op2, op3;
  if (printLevel == 1) {
    op1 = val->op1.vr;
    op2 = val->op2.vr;
    op3 = val->op3.vr;
  } else if (printLevel == 2) {
    op1 = val->op1.pr;
    op2 = val->op2.pr;
    op3 = val->op3.pr;
  } else {
    op1 = val->op1.sr;
    op2 = val->op2.sr;
    op3 = val->op3.sr;
  }

  switch (val->opcode) {
    case LOAD:
      printf("r%-7d", op1);
      printf("%-8s", "");
      printf("=> r%-4d", op3);
      break;
    case LOADI:
      printf("%-8d", val->op1.val);
      printf("%-8s", "");
      printf("=> r%-4d", op3);
      break;
    case STORE:
      printf("r%-7d", op1);
      printf("%-8s", "");
      printf("=> r%-4d", op2);
      break;
    case OUTPUT:
      printf("%-8d", val->op1.val);
      break;
    case NOP:
      break;
    default:
      printf("r%-7d", op1);
      printf(", r%-5d", op2);
      printf("=> r%-4d", op3);
      break;
  }

  printf("\n");

  return;
}

Inst* makeInst(Opcode opcode, Operand op1, Operand op2, Operand op3, int index) {
  Inst* val = (Inst*) malloc(sizeof(Inst));
  assertCondition(val != NULL, "Memory error allocating Inst.");
  val->opcode = opcode;
  val->op1 = op1;
  val->op2 = op2;
  val->op3 = op3;
  val->index = index;
  return val;
}
