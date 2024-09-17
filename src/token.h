
enum Category {
    INST, REG, CONST, COMMA, ARROW
};

typedef struct token {
    enum Category category;
    union {
        char* lexeme;
        int number;
    } value;
} Token;

void printToken(Token tok);
char* catToString(enum Category);