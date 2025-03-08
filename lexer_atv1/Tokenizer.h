#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <stdio.h>

/**
 * Tamanho máximo de uma linha no buffer
 */
#define MAX_BUFFER_SIZE 256

/**
 * Enumeração de categorias de tokens
 */
typedef enum {
    TOKEN_CATEGORY_NONE,    // Nenhuma categoria específica
    TOKEN_CATEGORY_MNEMONIC, // Instruções
    TOKEN_CATEGORY_NUMBER,   // Valores numéricos
    TOKEN_CATEGORY_DIRECTIVE, // Diretivas de montagem
    TOKEN_CATEGORY_EOL,      // Fim de linha
    TOKEN_CATEGORY_EOF       // Fim de arquivo
} TokenCategory;

/**
 * Estrutura que representa um token
 */
typedef struct {
    TokenCategory category;  // Categoria do token
    char* text;             // Texto do token
    int line_position;      // Número da linha
} LexToken;

/**
 * Estrutura do analisador léxico
 */
typedef struct {
    FILE* source_file;      // Arquivo de entrada
    char buffer[MAX_BUFFER_SIZE];  // Buffer de linha
    char* current_pos;      // Posição atual no buffer
    int current_line;       // Número da linha atual
    int comment_mode;       // Indicador de modo comentário
} TokenAnalyzer;

// Funções de gerenciamento do analisador
TokenAnalyzer* analyzer_create(FILE* src);
void analyzer_free(TokenAnalyzer* analyzer);

// Funções de análise e manipulação de tokens
LexToken* analyzer_get_next_token(TokenAnalyzer* analyzer);
void token_free(LexToken* token);

// Funções auxiliares de análise
int is_separator(char character);
int is_commentary(const char* text_line);
int is_hexadecimal(const char* text);
int hex_string_to_int(const char* hex_string);
int string_to_number(const char* text);

#endif // TOKENIZER_H
