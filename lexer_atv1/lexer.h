#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_UNKNOWN, TOKEN_COMMENT, TOKEN_DATA, TOKEN_CODE, TOKEN_INSTRUCTION, TOKEN_VARIABLE, TOKEN_NUMBER
} TokenType;

typedef struct {
    TokenType type;
    char value[100];
} Token;

typedef struct {
    FILE* source;
    char current_char;
    long current_pos;
} Scanner;

Scanner* create_scanner(FILE* source);
void advance(Scanner* scanner);
Token next_token(Scanner* scanner);

#endif // LEXER_H

