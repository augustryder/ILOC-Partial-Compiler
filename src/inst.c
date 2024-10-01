#include "inst.h"
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "types.h"

void printInst(Inst* val) {
  printf("(%s, %d, %d, %d)\t", opToString(val->opcode), val->sr1.val, val->sr2.val, val->dest.val);
  return;
}

void tPrintInst(Inst* val) {
  printf("| %-8s | %-8d | %-8d | %-8d |\n", opToString(val->opcode), val->sr1.val, val->sr2.val, val->dest.val);
  return;
}

void prettyPrintInst(Inst* val) {
  printf("%-8s", opToString(val->opcode));

  if (val->sr1.val == -1) {
    printf("\n");
    return; 
  }

  if (val->opcode == LOADI || val->opcode == OUTPUT) printf("%-8d", val->sr1.val);
  else printf("r%-7d", val->sr1.val);

  if (val->sr2.val == -1 || val->opcode == STORE) printf("%-8s", "");
  else printf(", r%-5d", val->sr2.val);

  if (val->dest.val == -1) {
    if (val->opcode == STORE) printf("=> r%-4d", val->sr2.val);
    printf("\n");
    return; 
  }
  else printf("=> r%-4d", val->dest.val);
  printf("\n");
  return;
}

Inst* makeInst(Opcode opcode, Operand sr1, Operand sr2, Operand dest) {
  Inst *val = (Inst*) malloc(sizeof(Inst));
  assertCondition(val != NULL, "Memory error allocating Inst.");
  val->opcode = opcode;
  val->sr1 = sr1;
  val->sr2 = sr2;
  val->dest = dest;
  return val;
}
