#ifndef NEANDER_H
#define NEANDER_H

#include <stdbool.h>
#include <stdint.h>

#define MEM_SIZE 516

// Instruction set
#define STORE 0x10
#define LOAD 0x20
#define SUM 0x30
#define INVERT 0x60
#define GOTO 0x80
#define IFNEG 0x90
#define IFZERO 0xA0
#define STOP 0xF0

typedef struct {
    uint8_t acc;
    uint8_t instr_ptr;
    bool zero;
    bool negative;
    uint8_t memory[MEM_SIZE];
} Neander;

void initialize_cpu(Neander *cpu, const char *filepath);
void run_cpu(Neander *cpu);
void display_cpu_memory(const Neander *cpu);

#endif // NEANDER_H

