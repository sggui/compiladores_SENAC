#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <stdio.h>
#include "Tokenizer.h"

/**
 * Tamanho máximo da memória do programa
 */
#define MEMORY_CAPACITY 512

/**
 * Estrutura que representa um código de operação
 */
typedef struct {
    char* mnemonic;          // Representação textual
    unsigned short opcode;   // Código de operação binário
} OpCodeEntry;

/**
 * Estrutura do montador
 */
typedef struct {
    TokenAnalyzer* analyzer;                  // Analisador léxico
    unsigned short program_memory[MEMORY_CAPACITY]; // Memória do programa
    int code_position;                        // Posição atual na seção de código
    int highest_address;                      // Endereço máximo utilizado
    int data_section_active;                  // Flag para seção de dados ativa
    const OpCodeEntry* opcodes;               // Tabela de códigos de operação
    int num_opcodes;                          // Quantidade de códigos na tabela
} Assembler;

// Funções de gerenciamento do montador
Assembler* assembler_create(TokenAnalyzer* analyzer, const OpCodeEntry* opcodes, int num_opcodes);
void assembler_free(Assembler* assembler);

// Funções principais do montador
int assembler_process(Assembler* assembler);
int assembler_generate_output(Assembler* assembler, const char* output_path);

// Função auxiliar para busca
int lookup_opcode(const OpCodeEntry* opcodes, int num_opcodes, const char* mnemonic);

#endif // ASSEMBLER_H
