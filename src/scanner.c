#include "scanner.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


// generates token for accepting state
Token s0(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s0.");
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
    }
}

Token s1(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s1.");
    }
    switch (c) {
        case 't': return s2to5(file);
        case 'u': return s6to7(file);
        default:
            error("Invalid transition s1.");
    }
}

Token s2to5(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s2.");
    }
    if (c == 'o' && getc(file) == 'r' && getc(file) == 'e' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.lexeme = "store";
        return tok;
    } else {
        error("Invalid transition somewhere s2 - s5");
    }
}

Token s6to7(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s6.");
    }
    if (c == 'b' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.lexeme = "store";
        return tok;
    } else {
        error("Invalid transition somewhere s2 - s5");
    }
}

Token s8(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s8.");
    }
    switch (c) {
        case 'o': return s9to12(file);
        case 's': return s14to18(file);
        default:
            error("Invalid transition s8.");
    }
}

Token s9to12(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s9.");
    }
    if (c == 'a' && getc(file) == 'd') {
        c = getc(file);
        if (isspace(c) || c == EOF) {
            Token tok;
            tok.category = INST;
            tok.value.lexeme = "load";
            return tok;
        } else if (c == 'I' && isspace(getc(file))) {
            Token tok;
            tok.category = INST;
            tok.value.lexeme = "loadI";
            return tok;
        } else {
            error("Invalid transition somewhere s11 - s12");
        }
    } else {
        error("Invalid transition somewhere s9 - s12");
    } 
}

Token s14to18(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s14.");
    }
    if (c == 'h' && getc(file) == 'i' && getc(file) == 'f' && getc(file) == 't' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.lexeme = "lshift";
        return tok;
    } else {
        error("Invalid transition somewhere s14 - s18");
    }
}

Token s13to18(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s13.");
    }
    if (c == 's' && getc(file) == 'h' && getc(file) == 'i' && getc(file) == 'f' && getc(file) == 't' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.lexeme = "rshift";
        return tok;
    } else {
        error("Invalid transition somewhere s13 - s18");
    } 
}

Token s19to18(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s19.");
    }
    if (c == 'u' && getc(file) == 'l' && getc(file) == 't' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.lexeme = "mult";
        return tok;
    } else {
        error("Invalid transition somewhere s19 - s18");
    } 
}

Token s22to24(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s22.");
    }
    if (c == 'd' && getc(file) == 'd' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.lexeme = "add";
        return tok;
    } else {
        error("Invalid transition somewhere s22 - s24");
    } 
}

Token s25to27(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s19.");
    }
    if (c == 'o' && getc(file) == 'p' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.lexeme = "nop";
        return tok;
    } else {
        error("Invalid transition somewhere s25 - s27");
    } 
}

Token s28to33(FILE* file) {
    char c = getc(file);
    if (c == EOF) {
        error("End of file s19.");
    }
    if (c == 'u' && getc(file) == 't' && getc(file) == 'p' && getc(file) == 'u' && getc(file) == 't' && isspace(getc(file))) {
        Token tok;
        tok.category = INST;
        tok.value.lexeme = "output";
        return tok;
    } else {
        error("Invalid transition somewhere s28 - s33");
    } 
}


