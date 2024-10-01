#include "types.h"
#include "utils.h"

char* catToString(Category cat) {
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

char* opToString(Opcode op) {
    switch (op) {
        case LOAD: return "LOAD";
        case LOADI: return "LOADI";
        case STORE: return "STORE";
        case ADD: return "ADD";
        case SUB: return "SUB";
        case MULT: return "MULT";
        case LSHIFT: return "LSHIFT";
        case RSHIFT: return "RSHIFT";
        case OUTPUT: return "OUTPUT";
        case NOP: return "NOP";
        default:
            error("Invalid opcode.");
    }
}
