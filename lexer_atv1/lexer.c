#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

Scanner* create_scanner(FILE* source) {
    Scanner* scanner = (Scanner*)malloc(sizeof(Scanner));
    scanner->source = source;
    scanner->current_pos = 0;
    scanner->current_char = fgetc(source);
    return scanner;
}

void advance(Scanner* scanner) {
    scanner->current_char = fgetc(scanner->source);
    scanner->current_pos++;
}

Token next_token(Scanner* scanner) {
    Token token = { TOKEN_UNKNOWN, "" };
    
    while (scanner->current_char != EOF && isspace(scanner->current_char)) {
        advance(scanner);
    }
    
    if (scanner->current_char == ';') {
        token.type = TOKEN_COMMENT;
        while (scanner->current_char != '\n' && scanner->current_char != EOF) {
            strncat(token.value, &scanner->current_char, 1);
            advance(scanner);
        }
    }
    else if (strncmp(&scanner->current_char, ".DATA", 5) == 0) {
        token.type = TOKEN_DATA;
        strcpy(token.value, ".DATA");
        advance(scanner); // Avança após o "."
    }
    else if (strncmp(&scanner->current_char, ".CODE", 5) == 0) {
        token.type = TOKEN_CODE;
        strcpy(token.value, ".CODE");
        advance(scanner);
    }
    else if (isalpha(scanner->current_char)) {
        token.type = TOKEN_INSTRUCTION;
        while (isalpha(scanner->current_char)) {
            strncat(token.value, &scanner->current_char, 1);
            advance(scanner);
        }
    }
    else if (isdigit(scanner->current_char)) {
        token.type = TOKEN_NUMBER;
        while (isdigit(scanner->current_char)) {
            strncat(token.value, &scanner->current_char, 1);
            advance(scanner);
        }
    }
    
    return token;
}

