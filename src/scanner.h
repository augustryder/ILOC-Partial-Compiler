#pragma once
#include <stdio.h>
#include "token.h"
#include "utils.h"

void printTokenStream(FILE* file);
Token nextToken(FILE* file);
Token s0(FILE* file);
Token s1(FILE* file);
Token s2to5(FILE* file);
Token s6to7(FILE* file);
Token s8(FILE* file);
Token s9to12(FILE* file);
Token s14to18(FILE* file);
Token s13to18(FILE* file);
Token s19to18(FILE* file);
Token s22to24(FILE* file);
Token s25to27(FILE* file);
Token s28to33(FILE* file);

Token getReg(FILE* file);
Token getConst(FILE* file);
Token getComma(FILE* file);
Token getArrow(FILE* file);
