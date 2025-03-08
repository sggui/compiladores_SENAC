#include <stdlib.h>
#include <string.h>
#include "Assembler.h"

/**
 * Inicializa um novo montador
 * @param analyzer Analisador léxico
 * @param opcodes Tabela de códigos de operação
 * @param num_opcodes Quantidade de códigos de operação
 * @return Ponteiro para estrutura do montador inicializada
 */
Assembler* assembler_create(TokenAnalyzer* analyzer, const OpCodeEntry* opcodes, int num_opcodes) {
    Assembler* assembler = (Assembler*)malloc(sizeof(Assembler));
    if (!assembler) return NULL;

    assembler->analyzer = analyzer;
    memset(assembler->program_memory, 0, MEMORY_CAPACITY * sizeof(unsigned short));
    assembler->code_position = 0;
    assembler->highest_address = 0;
    assembler->data_section_active = 0;
    assembler->opcodes = opcodes;
    assembler->num_opcodes = num_opcodes;

    return assembler;
}

/**
 * Libera recursos do montador
 * @param assembler Ponteiro para o montador
 */
void assembler_free(Assembler* assembler) {
    if (assembler) {
        free(assembler);
    }
}

/**
 * Busca um código de operação pelo mnemônico
 * @param opcodes Tabela de códigos de operação
 * @param num_opcodes Quantidade de códigos de operação
 * @param mnemonic Mnemônico a buscar
 * @return Índice do código de operação ou -1 se não encontrado
 */
int lookup_opcode(const OpCodeEntry* opcodes, int num_opcodes, const char* mnemonic) {
    for (int idx = 0; idx < num_opcodes; idx++) {
        if (strcmp(opcodes[idx].mnemonic, mnemonic) == 0) {
            return idx;
        }
    }
    return -1;
}

/**
 * Processa o arquivo fonte montando o programa
 * @param assembler Montador
 * @return 1 se sucesso, 0 se erro
 */
int assembler_process(Assembler* assembler) {
    LexToken* current_token;

    // Processa enquanto não atinge o fim do arquivo
    while ((current_token = analyzer_get_next_token(assembler->analyzer))->category != TOKEN_CATEGORY_EOF) {
        // Pula tokens de fim de linha
        if (current_token->category == TOKEN_CATEGORY_EOL) {
            token_free(current_token);
            continue;
        }

        // Processa diretivas de seção
        if (current_token->category == TOKEN_CATEGORY_DIRECTIVE) {
            if (strcmp(current_token->text, ".DATA") == 0) {
                assembler->data_section_active = 1;
                token_free(current_token);
                continue;
            } else if (strcmp(current_token->text, ".CODE") == 0) {
                assembler->data_section_active = 0;
                assembler->code_position = 0;
                token_free(current_token);
                continue;
            }
        }

        // Processamento específico para cada seção
        if (assembler->data_section_active) {
            // Processamento da seção .DATA
            if (current_token->category != TOKEN_CATEGORY_NUMBER) {
                fprintf(stderr, "Erro: Esperado número de endereço na seção DATA na linha %d\n",
                        current_token->line_position);
                token_free(current_token);
                return 0;
            }

            // Obtém o endereço
            int memory_addr = string_to_number(current_token->text);
            token_free(current_token);

            // Obtém o valor a ser armazenado
            current_token = analyzer_get_next_token(assembler->analyzer);
            if (current_token->category != TOKEN_CATEGORY_NUMBER) {
                fprintf(stderr, "Erro: Esperado valor após endereço na linha %d\n",
                        current_token->line_position);
                token_free(current_token);
                return 0;
            }

            // Armazena o valor no endereço indicado
            int memory_value = string_to_number(current_token->text);
            token_free(current_token);

            assembler->program_memory[memory_addr] = memory_value;

            // Atualiza endereço máximo utilizado
            if (memory_addr > assembler->highest_address) {
                assembler->highest_address = memory_addr;
            }
        } else {
            // Processamento da seção .CODE
            if (current_token->category != TOKEN_CATEGORY_MNEMONIC) {
                fprintf(stderr, "Erro: Esperado mnemônico na seção CODE na linha %d\n",
                        current_token->line_position);
                token_free(current_token);
                return 0;
            }

            // Busca código de operação
            int opcode_idx = lookup_opcode(assembler->opcodes, assembler->num_opcodes, current_token->text);
            if (opcode_idx == -1) {
                fprintf(stderr, "Erro: Mnemônico desconhecido '%s' na linha %d\n",
                        current_token->text, current_token->line_position);
                token_free(current_token);
                return 0;
            }

            // Armazena o código de operação
            assembler->program_memory[assembler->code_position] = assembler->opcodes[opcode_idx].opcode;

            // Verifica se a instrução precisa de operando
            if (strcmp(current_token->text, "NOP") != 0 && strcmp(current_token->text, "HLT") != 0) {
                token_free(current_token);

                // Obtém o operando
                current_token = analyzer_get_next_token(assembler->analyzer);
                if (current_token->category != TOKEN_CATEGORY_NUMBER) {
                    fprintf(stderr, "Erro: Esperado operando para instrução na linha %d\n",
                            current_token->line_position);
                    token_free(current_token);
                    return 0;
                }

                // Armazena o operando
                int operand_value = string_to_number(current_token->text);
                assembler->code_position++;
                assembler->program_memory[assembler->code_position] = operand_value;
            }

            token_free(current_token);
            assembler->code_position++;

            // Atualiza endereço máximo utilizado
            if (assembler->code_position > assembler->highest_address) {
                assembler->highest_address = assembler->code_position;
            }
        }
    }

    token_free(current_token);
    return 1;
}

/**
 * Grava o programa montado em arquivo de saída
 * @param assembler Montador
 * @param output_path Caminho do arquivo de saída
 * @return 1 se sucesso, 0 se erro
 */
int assembler_generate_output(Assembler* assembler, const char* output_path) {
    FILE* output_file;

    // Cria cópia temporária da memória
    unsigned short temp_memory[MEMORY_CAPACITY] = {0};
    for (int i = 0; i <= assembler->highest_address; i++) {
        temp_memory[i] = assembler->program_memory[i];
    }

    // Adiciona cabeçalho de identificação
    assembler->program_memory[0] = 0x4e03;
    assembler->program_memory[1] = 0x5244;

    // Copia a memória original após o cabeçalho
    for (int i = 0; i <= assembler->highest_address; i++) {
        assembler->program_memory[i + 2] = temp_memory[i];
    }

    // Ajusta o endereço máximo
    assembler->highest_address += 2;

    // Abre arquivo para escrita binária
    output_file = fopen(output_path, "wb");
    if (!output_file) {
        fprintf(stderr, "Erro ao abrir arquivo de saída: %s\n", output_path);
        return 0;
    }

    // Grava binário
    fwrite(assembler->program_memory, 2, assembler->highest_address + 1, output_file);
    fclose(output_file);

    // Gera arquivo de dump para depuração
    char dump_path[256];
    sprintf(dump_path, "%s.dump", output_path);
    FILE* dump_file = fopen(dump_path, "w");
    if (!dump_file) {
        fprintf(stderr, "Erro ao criar arquivo de dump: %s\n", dump_path);
        return 0;
    }

    // Arredonda para múltiplo de 4
    int max_address_rounded = (assembler->highest_address + 3) & ~3;

    // Garante tamanho mínimo
    if (max_address_rounded < 128) {
        max_address_rounded = 128;
    }

    // Gera arquivo de dump formatado
    for (int i = 0; i <= max_address_rounded; i += 4) {
        // Endereço base (em octal)
        fprintf(dump_file, "%07o", i * 2);

        // Valores (em hexadecimal)
        for (int j = 0; j < 4 && i + j <= max_address_rounded; j++) {
            fprintf(dump_file, " %04x", assembler->program_memory[i + j]);
        }

        fprintf(dump_file, "\n");

        // Compacta regiões de zeros
        int has_non_zero = 0;
        int next_block;
        for (next_block = i + 4; next_block <= max_address_rounded; next_block += 4) {
            for (int j = 0; j < 4 && next_block + j <= max_address_rounded; j++) {
                if (assembler->program_memory[next_block + j] != 0) {
                    has_non_zero = 1;
                    break;
                }
            }
            if (has_non_zero) break;
        }

        // Compacta blocos de zeros consecutivos
        if (!has_non_zero && i + 4 <= max_address_rounded) {
            fprintf(dump_file, "*\n");
            i = next_block - 4;
        }
    }

    // Marca fim do arquivo
    fprintf(dump_file, "%07o\n", (max_address_rounded + 1) * 2);

    fclose(dump_file);

    // Mensagens de conclusão
    printf("Montagem concluída com sucesso.\n");
    printf("Arquivo binário gerado: %s\n", output_path);
    printf("Arquivo de dump gerado: %s\n", dump_path);

    return 1;
}
