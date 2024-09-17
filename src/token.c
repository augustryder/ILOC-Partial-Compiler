#include "token.h"
#include "utils.h"

void printToken(Token tok) {
    if (tok.category == INST) {
        printf("<%s, %s>", catToString(tok.category), tok.value.lexeme);
    } else if (tok.category == REG || tok.category == CONST) {
        printf("<%s, %d>", catToString(tok.category), tok.value.number);
    } else {
        printf("<%s,>", catToString(tok.category));
    }
    printf("\n");
}

char* catToString(enum Category cat) {
    switch (cat) {
        case INST: return "INST";
        case REG: return "REG";
        case CONST: return "CONST";
        case COMMA: return "COMMA";
        case ARROW: return "ARROW";
        default:
            error("Invalid category.");
    }
}