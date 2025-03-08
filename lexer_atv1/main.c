#include <stdio.h>
#include <stdlib.h>
#include "Tokenizer.h"
#include "Assembler.h"

/**
 * Tabela de códigos de operação suportados
 */
OpCodeEntry instruction_set[] = {
    {"NOP", 0x0000}, // Nenhuma operação
    {"STA", 0x0010}, // Armazena acumulador
    {"LDA", 0x0020}, // Carrega acumulador
    {"ADD", 0x0030}, // Soma
    {"OR",  0x0040}, // OU lógico
    {"AND", 0x0050}, // E lógico
    {"NOT", 0x0060}, // Negação
    {"JMP", 0x0080}, // Desvio incondicional
    {"JN",  0x0090}, // Desvio se negativo
    {"JZ",  0x00A0}, // Desvio se zero
    {"HLT", 0x00F0}  // Finaliza execução
};

/**
 * Quantidade de instruções na tabela
 */
const int INSTRUCTION_COUNT = sizeof(instruction_set) / sizeof(OpCodeEntry);

/**
 * Ponto de entrada do programa
 */
int main(int argc, char* argv[]) {
    FILE* source_file;
    
    // Verifica parâmetros da linha de comando
    if (argc != 3) {
        fprintf(stderr, "Uso: %s arquivo_entrada arquivo_saida\n", argv[0]);
        return 1;
    }
    
    // Abre arquivo de entrada
    source_file = fopen(argv[1], "r");
    if (!source_file) {
        fprintf(stderr, "Erro ao abrir arquivo de entrada: %s\n", argv[1]);
        return 1;
    }
    
    // Inicializa componentes
    TokenAnalyzer* analyzer = analyzer_create(source_file);
    Assembler* assembler = assembler_create(analyzer, instruction_set, INSTRUCTION_COUNT);
    
    // Processa o arquivo fonte
    if (!assembler_process(assembler)) {
        fprintf(stderr, "Erro ao processar arquivo de entrada.\n");
        assembler_free(assembler);
        analyzer_free(analyzer);
        fclose(source_file);
        return 1;
    }
    
    // Gera o arquivo de saída
    if (!assembler_generate_output(assembler, argv[2])) {
        fprintf(stderr, "Erro ao gerar arquivo de saída.\n");
        assembler_free(assembler);
        analyzer_free(analyzer);
        fclose(source_file);
        return 1;
    }
    
    // Libera recursos
    assembler_free(assembler);
    analyzer_free(analyzer);
    fclose(source_file);
    
    return 0;
}
