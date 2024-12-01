#pragma once
#include "block.h"
#include <stdio.h>

Block* parse(FILE* file, int* blockSize);
Inst* getLoad(FILE* file, int index);
Inst* getLoadI(FILE* file, int index);
Inst* getStore(FILE* file, int index);
Inst* getArith(Opcode opcode, FILE* file, int index);
Inst* getOutput(FILE* file, int index);
Inst* getNop(FILE* file, int index);
