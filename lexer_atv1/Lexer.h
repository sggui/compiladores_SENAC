#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

typedef enum {
    TOKEN_INT,
    TOKEN_DATA,
    TOKEN_CODE,
    TOKEN_END,
    TOKEN_LDA,
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_IDENT,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    int value;
} Token;

Token next_token();
void executar_lexer(FILE *input);

#endif

