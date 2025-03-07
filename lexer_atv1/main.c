#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }
    
    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Error: Could not open input file %s\n", argv[1]);
        return 1;
    }
    
    Scanner* scanner = create_scanner(input_file);
    parse(scanner);

    fclose(input_file);

    // O código para gerar o output.mem seria aqui, conforme o que você desejar fazer após processar o arquivo.

    return 0;
}

