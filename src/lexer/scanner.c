#include "./scanner.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "token.h"
#include "../types.h"

Token nextToken(FILE* file) {
    skipWhitespace(file);
    char c = getc(file);
    if (c == 'r') {
        char next = peek(file);
        if (isNumber(next)) {
            ungetc(c, file);
            return getReg(file);
        } else if (isAlpha(next)) {
            ungetc(c, file);
            return s0(file);
        }
    } else if (isAlpha(c)) {
        ungetc(c, file);
        return s0(file);
    } else if (isNumber(c)) {
        ungetc(c, file);
        return getConst(file);
    } else if (c == ',') {
        ungetc(c, file);
        return getComma(file);
    } else if (c == '=' && peek(file) == '>') {
        ungetc(c, file);
        return getArrow(file);
    } else {
        error("Invalid symbol.");
        return;
    }
}

Token getReg(FILE* file) {
    char c = getc(file);
    if (c != 'r') {
        error("Not a valid register to get.");
        return;
    }
    int reg_number = 0;
    if (readNumber(file, &reg_number)) {
        Token tok;
        tok.category = REG;
        tok.value.number = reg_number;
        return tok;
    } else {
        error("Error reading register number.");
        return;
    }
}

Token getConst(FILE* file) {
    int constant = 0;
    if (readNumber(file, &constant)) {
        Token tok;
        tok.category = CONST;
        tok.value.number = constant;
        return tok;
    } else {
        error("Error reading constant.");
        return;
    }    
}

Token getComma(FILE* file) {
    char c = getc(file);
    if (c != ',') {
        error("No comma to get.");
        return;
    }
    Token tok;
    tok.category = COMMA;
    return tok;
}

Token getArrow(FILE* file) {
    char c = getc(file);
    if (getc(file) != '>' && c != '=') {
        error("No arrow to get.");
        return;
    }
    Token tok;
    tok.category = ARROW;
    return tok;
}

// generates token for accepting state in DFA
Token s0(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s0.");
        return;
    }
    switch (c) {
        case 's': return s1(file);
        case 'l': return s8(file);
        case 'r': return s13to18(file);
        case 'm': return s19to18(file);
        case 'a': return s22to24(file);
        case 'n': return s25to27(file);
        case 'o': return s28to33(file);
        default:
            error("Invalid transition s0.");
            return;
    }
}

Token s1(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s1.");
        return;
    }
    switch (c) {
        case 't': return s2to5(file);
        case 'u': return s6to7(file);
        default:
            error("Invalid transition s1.");
            return;
    }
}

Token s2to5(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s2.");
        return;
    }
    if (c == 'o' && getc(file) == 'r' && getc(file) == 'e' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.opcode = STORE;
        return tok;
    } else {
        error("Invalid transition somewhere s2 - s5");
        return;
    }
}

Token s6to7(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s6.");
        return;
    }
    if (c == 'b' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.opcode = SUB;
        return tok;
    } else {
        error("Invalid transition somewhere s2 - s5");
        return;
    }
}

Token s8(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s8.");
        return;
    }
    switch (c) {
        case 'o': return s9to12(file);
        case 's': return s14to18(file);
        default:
            error("Invalid transition s8.");
            return;
    }
}

Token s9to12(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s9.");
        return;
    }
    if (c == 'a' && getc(file) == 'd') {
        c = getc(file);
        if (isspace(c) || c == EOF) {
            Token tok;
            tok.category = INST;
            tok.value.opcode = LOAD;
            return tok;
        } else if (c == 'I' && isspace(getc(file))) {
            Token tok;
            tok.category = INST;
            tok.value.opcode = LOADI;
            return tok;
        } else {
            error("Invalid transition somewhere s11 - s12");
            return;
        }
    } else {
        error("Invalid transition somewhere s9 - s12");
        return;
    } 
}

Token s14to18(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s14.");
        return;
    }
    if (c == 'h' && getc(file) == 'i' && getc(file) == 'f' && getc(file) == 't' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.opcode = LSHIFT;
        return tok;
    } else {
        error("Invalid transition somewhere s14 - s18");
        return;
    }
}

Token s13to18(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s13.");
        return;
    }
    if (c == 's' && getc(file) == 'h' && getc(file) == 'i' && getc(file) == 'f' && getc(file) == 't' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.opcode = RSHIFT;
        return tok;
    } else {
        error("Invalid transition somewhere s13 - s18");
        return;
    } 
}

Token s19to18(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s19.");
        return;
    }
    if (c == 'u' && getc(file) == 'l' && getc(file) == 't' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.opcode = MULT;
        return tok;
    } else {
        error("Invalid transition somewhere s19 - s18");
        return;
    } 
}

Token s22to24(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s22.");
        return;
    }
    if (c == 'd' && getc(file) == 'd' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.opcode = ADD;
        return tok;
    } else {
        error("Invalid transition somewhere s22 - s24");
        return;
    } 
}

Token s25to27(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s19.");
        return;
    }
    if (c == 'o' && getc(file) == 'p' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.opcode = NOP;
        return tok;
    } else {
        error("Invalid transition somewhere s25 - s27");
        return;
    } 
}

Token s28to33(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s19.");
        return;
    }
    if (c == 'u' && getc(file) == 't' && getc(file) == 'p' && getc(file) == 'u' && getc(file) == 't' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.opcode = OUTPUT;
        return tok;
    } else {
        error("Invalid transition somewhere s28 - s33");
        return;
    } 
}
