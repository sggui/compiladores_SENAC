#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Definição dos tipos de token
typedef enum {
    TOKEN_INT,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_MAIS,
    TOKEN_MENOS,
    TOKEN_DIV,
    TOKEN_MULT,
    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

// Estrutura do Token
typedef struct {
    TokenType type;
    int value;
} Token;

// Função para obter o próximo token
Token next_token();

// Função para executar o lexer
void executar_lexer(FILE *input);

#endif
