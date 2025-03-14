// Inclusão de bibliotecas necessárias
#include <stdbool.h>  // Para usar tipos booleanos (true/false)
#include <stdint.h>   // Para usar tipos de inteiros com tamanho definido (uint8_t)
#include <stdio.h>    // Para operações de entrada/saída
#include <string.h>   // Para manipulação de strings

// Definição de constantes para os códigos de instrução do processador simulado
#define STORE 0x10    // Armazena valor na memória
#define LOAD 0x20     // Carrega valor da memória para o acumulador
#define SUM 0x30      // Soma valor da memória ao acumulador
#define INVERT 0x60   // Inverte todos os bits do acumulador
#define GOTO 0x80     // Salta para outro endereço de instrução
#define IFNEG 0x90    // Salta condicional se o acumulador for negativo
#define IFZERO 0xA0   // Salta condicional se o acumulador for zero
#define STOP 0xF0     // Termina a execução do programa

// Função para exibir o estado atual da memória e registradores
void display_memory(uint8_t *memory, size_t length, uint8_t acc, uint8_t instr_ptr, bool zero, bool negative) {
  size_t position = 0;
  // Mostra o valor do acumulador e do ponteiro de instrução
  printf("ACC: %03d  | IP: %03d\n", acc, instr_ptr);
  // Mostra os flags de zero e negativo
  printf("ZERO: %s | NEG: %s\n", zero ? "true" : "false", negative ? "true" : "false");
  
  // Exibe o conteúdo da memória em formato hexadecimal, 16 bytes por linha
  while (position < length) {
    printf("%08zx: ", position);  // Endereço da linha atual
    for (size_t i = 0; i < 16; i++) {
      if (position + i < length)
        printf("%02x ", memory[position + i]);  // Exibe byte da memória em formato hex
      else
        printf("   ");  // Espaço para alinhar quando não há mais bytes
    }
    printf("\n");
    position += 16;  // Avança para a próxima linha
  }
  position += 16;
  printf("\n\n");
}

int main(int argc, char const *argv[]) {
  // Verifica se foi fornecido um arquivo como argumento
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return 1;
  }
  
  // Inicialização dos registradores e flags
  uint8_t acc = 0, instr_ptr = 0;  // Acumulador e ponteiro de instrução
  bool zero = 0, negative = 0;     // Flags de status
  
  const char *file_path = argv[1];  // Caminho do arquivo do programa
  FILE *binary_file = fopen(file_path, "rb");  // Abre o arquivo binário
  if (!binary_file) {
    perror("Error opening file");
    return 1;
  }
  
  // Aloca memória para o programa e dados
  uint8_t memory[516];  // 4 bytes de header + 512 bytes de programa/dados
  uint8_t header[4];    // Cabeçalho do arquivo
  
  // Lê o cabeçalho do arquivo (4 bytes)
  fread(header, 1, 4, binary_file);
  // Lê o resto do arquivo (512 bytes) para a memória a partir do endereço 4
  fread(memory + 4, 1, 512, binary_file);
  fclose(binary_file);
  
  // Exibe o estado inicial da memória e registradores
  display_memory(memory, 516, acc, instr_ptr, zero, negative);
  
  // Loop principal de execução do programa
  while (memory[instr_ptr] != STOP) {  // Continua até encontrar instrução STOP
    // Atualiza as flags de status baseadas no acumulador
    zero = (acc == 0);
    negative = (acc & 0x80) != 0;  // Bit mais significativo indica número negativo
    
    // Interpreta a instrução atual
    switch (memory[instr_ptr]) {
    case 0x00:  // Instrução NOP (No Operation) - não faz nada
      break;
    case STORE:  // Armazena acumulador na memória
      memory[memory[instr_ptr + 2] * 2 + 4] = acc;
      break;
    case LOAD:  // Carrega valor da memória para o acumulador
      acc = memory[memory[instr_ptr + 2] * 2 + 4];
      break;
    case SUM:  // Soma valor da memória ao acumulador
      acc += memory[memory[instr_ptr + 2] * 2 + 4];
      break;
    case 0x40:  // Operação OR bit a bit
      acc = memory[memory[instr_ptr + 2] * 2 + 4] | acc;
      break;
    case 0x50:  // Operação AND bit a bit
      acc = memory[memory[instr_ptr + 2] * 2 + 4] & acc;
      break;
    case INVERT:  // Inverte todos os bits do acumulador
      acc = ~acc;
      instr_ptr += 2;  // Avança apenas 2 posições (instrução de 2 bytes)
      continue;
    case GOTO:  // Salta para outro endereço
      instr_ptr = memory[instr_ptr + 2] * 2 + 4;  // Calcula novo endereço
      continue;  // Salta a atualização normal do ponteiro de instrução
    case IFNEG:  // Salta se acumulador for negativo
      if (negative) {
        instr_ptr = memory[instr_ptr + 2] * 2 + 4;
        continue;  // Salta a atualização normal do ponteiro de instrução
      }
      break;
    case IFZERO:  // Salta se acumulador for zero
      if (zero) {
        instr_ptr = memory[instr_ptr + 2] * 2 + 4;
        continue;  // Salta a atualização normal do ponteiro de instrução
      }
      break;
    }
    
    // Avança para a próxima instrução (cada instrução tem 4 bytes)
    instr_ptr += 4;
    // Exibe o estado atual após executar a instrução
    display_memory(memory, 516, acc, instr_ptr, zero, negative);
  }
  
  // Exibe o estado final após término do programa
  display_memory(memory, 516, acc, instr_ptr, zero, negative);
  return 0;
}
