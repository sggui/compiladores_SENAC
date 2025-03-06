#include "Lexer.h"

static int lookahead;
static FILE *source;

Token next_token() {
    while (isspace(lookahead))
        lookahead = fgetc(source);

    switch (lookahead) {
        case '(':
            lookahead = fgetc(source);
            return (Token){TOKEN_LPAREN, 0};
        case ')':
            lookahead = fgetc(source);
            return (Token){TOKEN_RPAREN, 0};
        case '+':
            lookahead = fgetc(source);
            return (Token){TOKEN_MAIS, 0};
        case '-':
            lookahead = fgetc(source);
            return (Token){TOKEN_MENOS, 0};
        case '/':
            lookahead = fgetc(source);
            return (Token){TOKEN_DIV, 0};
        case '*':
            lookahead = fgetc(source);
            return (Token){TOKEN_MULT, 0};
        case EOF:
            return (Token){TOKEN_EOF, 0};
        default:
            if (isdigit(lookahead)) {
                int value = 0;
                while (isdigit(lookahead)) {
                    value = value * 10 + (lookahead - '0');
                    lookahead = fgetc(source);
                }
                return (Token){TOKEN_INT, value};
            } else {
                fprintf(stderr, "Erro: Caractere inesperado: %c\n", lookahead);
                exit(EXIT_FAILURE);
            }
    }
}

void executar_lexer(FILE *input) {
    source = input;
    lookahead = fgetc(source);
    Token token;

    while (1) {
        token = next_token();
        switch (token.type) {
            case TOKEN_INT:
                printf("Número inteiro: %d\n", token.value);
                break;
            case TOKEN_LPAREN:
                printf("Símbolo de parênteses esquerdo: (\n");
                break;
            case TOKEN_RPAREN:
                printf("Símbolo de parênteses direito: )\n");
                break;
            case TOKEN_MAIS:
                printf("Símbolo de soma: +\n");
                break;
            case TOKEN_MENOS:
                printf("Símbolo de subtração: -\n");
                break;
            case TOKEN_DIV:
                printf("Símbolo de divisão: /\n");
                break;
            case TOKEN_MULT:
                printf("Símbolo de multiplicação: *\n");
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
