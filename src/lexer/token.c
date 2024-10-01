#include "./token.h"
#include "../utils.h"
#include "../types.h"

void printToken(Token tok) {
    if (tok.category == INST) {
        printf("<%s, %s>", catToString(tok.category), opToString(tok.value.opcode));
    } else if (tok.category == REG || tok.category == CONST) {
        printf("<%s, %d>", catToString(tok.category), tok.value.number);
    } else {
        printf("<%s>", catToString(tok.category));
    }
    printf("\n");
}
