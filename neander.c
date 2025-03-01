#include "neander.h"
#include <stdio.h>
#include <stdlib.h>

void initialize_cpu(Neander *cpu, const char *filepath) {
    cpu->acc = 0;
    cpu->instr_ptr = 0;
    cpu->zero = false;
    cpu->negative = false;
    
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        printf("Error opening file.\n");
        exit(1);
    }

    fread(cpu->memory, 1, MEM_SIZE, file);
    fclose(file);
}

void run_cpu(Neander *cpu) {
    while (cpu->memory[cpu->instr_ptr] != STOP) {
        uint8_t operand = cpu->memory[cpu->instr_ptr + 2];
        uint8_t address = operand * 2 + 4;

        switch (cpu->memory[cpu->instr_ptr]) {
            case STORE:
                cpu->memory[address] = cpu->acc;
                break;
            case LOAD:
                cpu->acc = cpu->memory[address];
                break;
            case SUM:
                cpu->acc += cpu->memory[address];
                break;
            case INVERT:
                cpu->acc = ~cpu->acc;
                break;
            case GOTO:
                cpu->instr_ptr = address;
                continue;
            case IFNEG:
                if (cpu->negative) {
                    cpu->instr_ptr = address;
                    continue;
                }
                break;
            case IFZERO:
                if (cpu->zero) {
                    cpu->instr_ptr = address;
                    continue;
                }
                break;
            default:
                break;
        }

        cpu->zero = (cpu->acc == 0);
        cpu->negative = (cpu->acc & 0x80) != 0;
        cpu->instr_ptr += 4;
    }
}

void display_cpu_memory(const Neander *cpu) {
    printf("\n\nACC: %03d  | IP: %03d\n", cpu->acc, cpu->instr_ptr);
    printf("ZERO: %s | NEG: %s\n", cpu->zero ? "true" : "false", cpu->negative ? "true" : "false");

    for (size_t offset = 0; offset < MEM_SIZE; offset += 16) {
        printf("%08zx: ", offset);
        for (size_t i = 0; i < 16; i++) {
            if (offset + i < MEM_SIZE)
                printf("%02x ", cpu->memory[offset + i]);
            else
                printf("   ");
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    Neander cpu;
    initialize_cpu(&cpu, argv[1]);
    display_cpu_memory(&cpu);
    run_cpu(&cpu);
    display_cpu_memory(&cpu);

    return 0;
}
