#pragma once
#include "block.h"
#include <stdio.h>

Block* parse(FILE* file);
Inst* getLoad(FILE* file);
Inst* getLoadI(FILE* file);
Inst* getStore(FILE* file);
Inst* getArith(Opcode opcode, FILE* file);
Inst* getOutput(FILE* file);
Inst* getNop(FILE* file);
