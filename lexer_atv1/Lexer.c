#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static int lookahead;
static FILE *source;

Token next_token() {
    while (isspace(lookahead) || lookahead == ',')  // Ignorar espaços e vírgulas
        lookahead = fgetc(source);

    if (isalpha(lookahead)) {  // Identificar palavras-chave e comandos
        char buffer[32];
        int i = 0;
        while (isalpha(lookahead)) {
            buffer[i++] = lookahead;
            lookahead = fgetc(source);
        }
        buffer[i] = '\0';

        if (strcmp(buffer, "DATA") == 0) return (Token){TOKEN_DATA, 0};
        if (strcmp(buffer, "END") == 0) return (Token){TOKEN_END, 0};
        if (strcmp(buffer, "CODE") == 0) return (Token){TOKEN_CODE, 0};
        if (strcmp(buffer, "LDA") == 0) return (Token){TOKEN_LDA, 0};
        if (strcmp(buffer, "ADD") == 0) return (Token){TOKEN_ADD, 0};
        if (strcmp(buffer, "SUB") == 0) return (Token){TOKEN_SUB, 0};
        if (strcmp(buffer, "MUL") == 0) return (Token){TOKEN_MUL, 0};
        if (strcmp(buffer, "DIV") == 0) return (Token){TOKEN_DIV, 0};

        return (Token){TOKEN_IDENT, 0};  // Identificadores desconhecidos
    }

    if (isdigit(lookahead)) {  // Identificar números inteiros
        int value = 0;
        while (isdigit(lookahead)) {
            value = value * 10 + (lookahead - '0');
            lookahead = fgetc(source);
        }
        return (Token){TOKEN_INT, value};
    }

    if (lookahead == ';') {  // Ignorar ';' como separador de comandos
        lookahead = fgetc(source);
        return next_token();
    }

    if (lookahead == EOF)  // Fim do arquivo
        return (Token){TOKEN_EOF, 0};

    fprintf(stderr, "Erro: Caractere inesperado: %c\n", lookahead);
    exit(EXIT_FAILURE);
}

void executar_lexer(FILE *input) {
    source = input;
    lookahead = fgetc(source);
    Token token;

    while (1) {
        token = next_token();
        switch (token.type) {
            case TOKEN_DATA:
                printf("Seção DATA\n");
                break;
            case TOKEN_CODE:
                printf("Seção CODE\n");
                break;
            case TOKEN_END:
                printf("Fim de seção\n");
                break;
            case TOKEN_LDA:
                printf("Comando: LDA\n");
                break;
            case TOKEN_ADD:
                printf("Comando: ADD\n");
                break;
            case TOKEN_SUB:
                printf("Comando: SUB\n");
                break;
            case TOKEN_MUL:
                printf("Comando: MUL\n");
                break;
            case TOKEN_DIV:
                printf("Comando: DIV\n");
                break;
            case TOKEN_INT:
                printf("Número inteiro: %d\n", token.value);
                break;
            case TOKEN_IDENT:
                printf("Identificador desconhecido\n");
                break;
            case TOKEN_EOF:
                printf("Fim de arquivo\n");
                return;
            default:
                fprintf(stderr, "Erro: Token inválido\n");
                exit(EXIT_FAILURE);
        }
    }
}

