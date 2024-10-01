#include "inst.h"
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "../types.h"

void printInst(Inst* val) {
  printf("(%s, %d, %d, %d)\t", opToString(val->opcode), val->sr1.val, val->sr2.val, val->dest.val);
  return;
}

void tPrintInst(Inst* val) {
  printf("| %s | %8d | %8d | %8d |\n", opToString(val->opcode), val->sr1.val, val->sr2.val, val->dest.val);
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
