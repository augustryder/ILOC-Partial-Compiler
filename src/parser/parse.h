#pragma once
#include "./list.h"
#include <stdio.h>

List* parse(FILE* file);
Inst* getLoad(FILE* file);
Inst* getLoadI(FILE* file);
Inst* getStore(FILE* file);
Inst* getArith(FILE* file);
Inst* getOutput(FILE* file);
Inst* getNop(FILE* file);
