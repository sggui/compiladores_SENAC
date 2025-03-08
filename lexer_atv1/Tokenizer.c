#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Tokenizer.h"

/**
 * Inicializa um novo analisador léxico
 * @param src Arquivo de entrada
 * @return Ponteiro para estrutura do analisador inicializada
 */
TokenAnalyzer* analyzer_create(FILE* src) {
    TokenAnalyzer* analyzer = (TokenAnalyzer*)malloc(sizeof(TokenAnalyzer));
    if (!analyzer) return NULL;

    analyzer->source_file = src;
    analyzer->buffer[0] = '\0';
    analyzer->current_pos = analyzer->buffer;
    analyzer->current_line = 0;
    analyzer->comment_mode = 0;

    return analyzer;
}

/**
 * Libera recursos do analisador léxico
 * @param analyzer Ponteiro para o analisador
 */
void analyzer_free(TokenAnalyzer* analyzer) {
    if (analyzer) {
        free(analyzer);
    }
}

/**
 * Verifica se um caractere é separador
 * @param character Caractere a verificar
 * @return 1 se for separador, 0 caso contrário
 */
int is_separator(char character) {
    return (character == ' ' || character == '\t' || character == '\n' || 
            character == '\r' || character == ';');
}

/**
 * Verifica se uma linha é comentário
 * @param text_line Linha de texto a verificar
 * @return 1 se for comentário, 0 caso contrário
 */
int is_commentary(const char* text_line) {
    // Ignora espaços em branco no início
    while (*text_line && (*text_line == ' ' || *text_line == '\t')) {
        text_line++;
    }

    // Verifica se começa com símbolo de comentário
    return (*text_line == ';');
}

/**
 * Verifica se uma string é número hexadecimal
 * @param text String a verificar
 * @return 1 se for hexadecimal, 0 caso contrário
 */
int is_hexadecimal(const char* text) {
    // Pula prefixo 0x ou 0X se existir
    if (text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) {
        text += 2;
    }

    // Verifica se todos os caracteres são válidos em hexadecimal
    while (*text) {
        if (!((*text >= '0' && *text <= '9') ||
              (*text >= 'a' && *text <= 'f') ||
              (*text >= 'A' && *text <= 'F'))) {
            return 0;
        }
        text++;
    }

    return 1;
}

/**
 * Converte uma string hexadecimal para inteiro
 * @param hex_string String hexadecimal
 * @return Valor inteiro equivalente
 */
int hex_string_to_int(const char* hex_string) {
    int value = 0;
    while (*hex_string) {
        value *= 16;
        if (*hex_string >= '0' && *hex_string <= '9') {
            value += *hex_string - '0';
        } else if (*hex_string >= 'A' && *hex_string <= 'F') {
            value += *hex_string - 'A' + 10;
        } else if (*hex_string >= 'a' && *hex_string <= 'f') {
            value += *hex_string - 'a' + 10;
        }
        hex_string++;
    }
    return value;
}

/**
 * Analisa uma string e converte para número
 * @param text String a converter
 * @return Valor numérico
 */
int string_to_number(const char* text) {
    // Lida com prefixo hexadecimal explícito
    if (text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) {
        return strtol(text + 2, NULL, 16);
    }

    // Verifica se é hexadecimal sem prefixo
    if (is_hexadecimal(text)) {
        return hex_string_to_int(text);
    }

    // Caso contrário, trata como decimal
    return atoi(text);
}

/**
 * Obtém o próximo token do analisador
 * @param analyzer Analisador léxico
 * @return Ponteiro para o token encontrado
 */
LexToken* analyzer_get_next_token(TokenAnalyzer* analyzer) {
    LexToken* token = (LexToken*)malloc(sizeof(LexToken));
    if (!token) return NULL;

    token->category = TOKEN_CATEGORY_NONE;
    token->text = NULL;
    token->line_position = analyzer->current_line;

    // Verifica se precisa ler mais uma linha
    if (*(analyzer->current_pos) == '\0') {
        if (fgets(analyzer->buffer, MAX_BUFFER_SIZE, analyzer->source_file) == NULL) {
            token->category = TOKEN_CATEGORY_EOF;
            return token;
        }

        analyzer->current_pos = analyzer->buffer;
        analyzer->current_line++;

        // Ignora linhas de comentário
        if (is_commentary(analyzer->buffer)) {
            free(token);
            return analyzer_get_next_token(analyzer);
        }

        token->line_position = analyzer->current_line;
    }

    // Pula caracteres separadores
    while (*(analyzer->current_pos) && is_separator(*(analyzer->current_pos))) {
        if (*(analyzer->current_pos) == ';') {
            *(analyzer->current_pos) = '\0';
            token->category = TOKEN_CATEGORY_EOL;
            return token;
        }
        (analyzer->current_pos)++;
    }

    // Verifica fim da linha atual
    if (*(analyzer->current_pos) == '\0') {
        token->category = TOKEN_CATEGORY_EOL;
        return token;
    }

    // Marca o início do token
    char* token_begin = analyzer->current_pos;

    // Avança até o fim do token
    while (*(analyzer->current_pos) && !is_separator(*(analyzer->current_pos))) {
        (analyzer->current_pos)++;
    }

    // Salva o caractere após o token e substitui por nulo temporariamente
    char temp_char = *(analyzer->current_pos);
    if (*(analyzer->current_pos)) {
        *(analyzer->current_pos) = '\0';
        (analyzer->current_pos)++;
    }

    // Identifica a categoria do token
    if (token_begin[0] == '.') {
        token->category = TOKEN_CATEGORY_DIRECTIVE;
    } else if (isdigit(token_begin[0]) ||
              (token_begin[0] == '0' && (token_begin[1] == 'x' || token_begin[1] == 'X'))) {
        token->category = TOKEN_CATEGORY_NUMBER;
    } else {
        token->category = TOKEN_CATEGORY_MNEMONIC;
    }

    // Copia o texto do token
    token->text = strdup(token_begin);

    // Restaura o caractere original
    if (temp_char != '\0') {
        *(analyzer->current_pos - 1) = temp_char;
    }

    return token;
}

/**
 * Libera os recursos de um token
 * @param token Token a ser liberado
 */
void token_free(LexToken* token) {
    if (token) {
        if (token->text) {
            free(token->text);
        }
        free(token);
    }
}
