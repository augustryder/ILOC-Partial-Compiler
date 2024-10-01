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
        case LOAD: return "load";
        case LOADI: return "loadI";
        case STORE: return "store";
        case ADD: return "add";
        case SUB: return "sub";
        case MULT: return "mult";
        case LSHIFT: return "lshift";
        case RSHIFT: return "rshift";
        case OUTPUT: return "output";
        case NOP: return "nop";
        default:
            error("Invalid opcode.");
    }
}
