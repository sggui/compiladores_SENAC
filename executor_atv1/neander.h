#ifndef NEANDER_H      // Guarda de inclusão - impede inclusões múltiplas do arquivo de cabeçalho
#define NEANDER_H

#include <stdbool.h>   // Para o tipo booleano (bool, true, false)
#include <stdint.h>    // Para tipos inteiros com tamanho fixo (uint8_t)

#define MEM_SIZE 516   // Define o tamanho da memória da máquina Neander (516 bytes)

// Definição das constantes para o conjunto de instruções da máquina Neander
#define STORE 0x10     // Armazena o valor do acumulador em um endereço de memória
#define LOAD 0x20      // Carrega um valor da memória para o acumulador
#define SUM 0x30       // Soma um valor da memória ao acumulador
#define INVERT 0x60    // Inverte todos os bits do acumulador (complemento de 1)
#define GOTO 0x80      // Salto incondicional para um endereço de memória
#define IFNEG 0x90     // Salto condicional se o acumulador for negativo
#define IFZERO 0xA0    // Salto condicional se o acumulador for zero
#define STOP 0xF0      // Encerra a execução do programa

// Estrutura que representa o estado completo da máquina Neander
typedef struct {
   uint8_t acc;       // Acumulador - registrador de 8 bits para operações aritméticas
   uint8_t instr_ptr; // Ponteiro de instrução - indica endereço da instrução atual
   bool zero;         // Flag que indica se o acumulador é zero
   bool negative;     // Flag que indica se o acumulador é negativo
   uint8_t memory[MEM_SIZE]; // Memória da máquina (516 bytes)
} Neander;

// Declarações das funções para manipular a máquina Neander
// Inicializa a CPU e carrega um programa do arquivo
void initialize_cpu(Neander *cpu, const char *filepath);

// Executa o programa carregado na memória
void run_cpu(Neander *cpu);

// Exibe o estado atual da CPU e o conteúdo da memória
void display_cpu_memory(const Neander *cpu);

#endif // NEANDER_H    // Encerra a guarda de inclusão
