#include "neander.h" // Inclui o arquivo de cabeçalho com definições da máquina Neander
#include <stdio.h>   // Para funções de entrada/saída
#include <stdlib.h>  // Para a função exit()

// Inicializa a CPU Neander com valores padrão e carrega o programa do arquivo
void initialize_cpu(Neander *cpu, const char *filepath) {
    // Inicializa os registradores e flags com valores padrão
    cpu->acc = 0;           // Acumulador zerado
    cpu->instr_ptr = 0;     // Ponteiro de instrução no início
    cpu->zero = false;      // Flag zero desativada
    cpu->negative = false;  // Flag negativo desativada
    
    // Abre o arquivo binário do programa para leitura
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        printf("Error opening file.\n");
        exit(1);  // Encerra o programa se não conseguir abrir o arquivo
    }
    // Lê o conteúdo do arquivo diretamente para a memória da CPU
    fread(cpu->memory, 1, MEM_SIZE, file);
    fclose(file);  // Fecha o arquivo após a leitura
}

// Executa o programa carregado na memória da CPU
void run_cpu(Neander *cpu) {
    // Loop principal que executa até encontrar a instrução STOP
    while (cpu->memory[cpu->instr_ptr] != STOP) {
        // Extrai o operando da instrução atual (terceiro byte da instrução)
        uint8_t operand = cpu->memory[cpu->instr_ptr + 2];
        // Calcula o endereço efetivo na memória baseado no operando
        uint8_t address = operand * 2 + 4;
        
        // Interpreta e executa a instrução atual baseado no opcode
        switch (cpu->memory[cpu->instr_ptr]) {
            case STORE:  // Armazena o valor do acumulador na memória
                cpu->memory[address] = cpu->acc;
                break;
            case LOAD:   // Carrega um valor da memória para o acumulador
                cpu->acc = cpu->memory[address];
                break;
            case SUM:    // Soma um valor da memória ao acumulador
                cpu->acc += cpu->memory[address];
                break;
            case INVERT: // Inverte todos os bits do acumulador
                cpu->acc = ~cpu->acc;
                break;
            case GOTO:   // Salta para outro endereço de instrução
                cpu->instr_ptr = address;
                continue;  // Pula a atualização do ponteiro de instrução
            case IFNEG:  // Salta condicional se o acumulador for negativo
                if (cpu->negative) {
                    cpu->instr_ptr = address;
                    continue;  // Pula a atualização do ponteiro de instrução
                }
                break;
            case IFZERO: // Salta condicional se o acumulador for zero
                if (cpu->zero) {
                    cpu->instr_ptr = address;
                    continue;  // Pula a atualização do ponteiro de instrução
                }
                break;
            default:     // Instruções não reconhecidas são ignoradas
                break;
        }
        
        // Atualiza as flags de status baseadas no valor atual do acumulador
        cpu->zero = (cpu->acc == 0);
        cpu->negative = (cpu->acc & 0x80) != 0;  // Bit mais significativo indica número negativo
        
        // Avança para a próxima instrução (cada instrução ocupa 4 bytes)
        cpu->instr_ptr += 4;
    }
}

// Exibe o estado atual da CPU e o conteúdo da memória
void display_cpu_memory(const Neander *cpu) {
    // Mostra o valor do acumulador e do ponteiro de instrução
    printf("\n\nACC: %03d  | IP: %03d\n", cpu->acc, cpu->instr_ptr);
    // Mostra o estado das flags zero e negativo
    printf("ZERO: %s | NEG: %s\n", cpu->zero ? "true" : "false", cpu->negative ? "true" : "false");
    
    // Exibe o conteúdo da memória em formato hexadecimal, 16 bytes por linha
    for (size_t offset = 0; offset < MEM_SIZE; offset += 16) {
        printf("%08zx: ", offset);  // Endereço da linha atual
        for (size_t i = 0; i < 16; i++) {
            if (offset + i < MEM_SIZE)
                printf("%02x ", cpu->memory[offset + i]);  // Exibe byte em formato hex
            else
                printf("   ");  // Espaço para alinhar quando não há mais bytes
        }
        printf("\n");
    }
}

// Função principal do programa
int main(int argc, char **argv) {
    // Verifica se foi fornecido um arquivo como argumento
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    // Cria uma instância da CPU Neander
    Neander cpu;
    
    // Inicializa a CPU com o arquivo fornecido
    initialize_cpu(&cpu, argv[1]);
    
    // Exibe o estado inicial da CPU e memória
    display_cpu_memory(&cpu);
    
    // Executa o programa carregado na CPU
    run_cpu(&cpu);
    
    // Exibe o estado final da CPU e memória após a execução
    display_cpu_memory(&cpu);
    
    return 0;
}
