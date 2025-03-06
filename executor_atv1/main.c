#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define STORE 0x10
#define LOAD 0x20
#define SUM 0x30
#define INVERT 0x60
#define GOTO 0x80
#define IFNEG 0x90
#define IFZERO 0xA0
#define STOP 0xF0

void display_memory(uint8_t *memory, size_t length, uint8_t acc, uint8_t instr_ptr, bool zero, bool negative) {
  size_t position = 0;

  printf("ACC: %03d  | IP: %03d\n", acc, instr_ptr);
  printf("ZERO: %s | NEG: %s\n", zero ? "true" : "false", negative ? "true" : "false");

  while (position < length) {
    printf("%08zx: ", position);

    for (size_t i = 0; i < 16; i++) {
      if (position + i < length)
        printf("%02x ", memory[position + i]);
      else
        printf("   ");
    }

    printf("\n");
    position += 16;
  }

  position += 16;

  printf("\n\n");
}

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  uint8_t acc = 0, instr_ptr = 0;
  bool zero = 0, negative = 0;

  const char *file_path = argv[1];
  FILE *binary_file = fopen(file_path, "rb");
  if (!binary_file) {
    perror("Error opening file");
    return 1;
  }

  uint8_t memory[516];
  uint8_t header[4];
  fread(header, 1, 4, binary_file);

  fread(memory + 4, 1, 512, binary_file);
  fclose(binary_file);

  display_memory(memory, 516, acc, instr_ptr, zero, negative);

  while (memory[instr_ptr] != STOP) {

    zero = (acc == 0);
    negative = (acc & 0x80) != 0;

    switch (memory[instr_ptr]) {
    case 0x00:
      break;
    case STORE:
      memory[memory[instr_ptr + 2] * 2 + 4] = acc;
      break;
    case LOAD:
      acc = memory[memory[instr_ptr + 2] * 2 + 4];
      break;
    case SUM:
      acc += memory[memory[instr_ptr + 2] * 2 + 4];
      break;
    case 0x40:
      acc = memory[memory[instr_ptr + 2] * 2 + 4] | acc;
      break;
    case 0x50:
      acc = memory[memory[instr_ptr + 2] * 2 + 4] & acc;
      break;
    case INVERT:
      acc = ~acc;
      instr_ptr += 2;
      continue;
    case GOTO:
      instr_ptr = memory[instr_ptr + 2] * 2 + 4;
      continue;
    case IFNEG:
      if (negative) {
        instr_ptr = memory[instr_ptr + 2] * 2 + 4;
        continue;
      }
      break;
    case IFZERO:
      if (zero) {
        instr_ptr = memory[instr_ptr + 2] * 2 + 4;
        continue;
      }
      break;
    }

    instr_ptr += 4;
    display_memory(memory, 516, acc, instr_ptr, zero, negative);
  }

  display_memory(memory, 516, acc, instr_ptr, zero, negative);

  return 0;
}

