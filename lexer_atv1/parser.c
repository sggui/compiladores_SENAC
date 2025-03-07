#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

void parse(Scanner* scanner) {
    Token token = next_token(scanner);
    
    while (token.type != TOKEN_UNKNOWN) {
        if (token.type == TOKEN_COMMENT) {
            printf("Comment: %s\n", token.value);
        }
        else if (token.type == TOKEN_DATA) {
            printf("Processing .DATA section\n");
            // Processar dados
        }
        else if (token.type == TOKEN_CODE) {
            printf("Processing .CODE section\n");
            // Processar instruções
        }
        else if (token.type == TOKEN_INSTRUCTION) {
            printf("Processing Instruction: %s\n", token.value);
            // Processar instrução
        }
        else if (token.type == TOKEN_NUMBER) {
            printf("Processing Number: %s\n", token.value);
            // Processar número (como uma variável ou valor)
        }
        
        token = next_token(scanner);
    }
}

