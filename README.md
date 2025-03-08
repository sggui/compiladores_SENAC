## **NEANDER (EXECUTOR)**

- Clonar o repositório:
```sh
git clone https://github.com/sggui/compiladores_SENAC.git
```
- Entrar na pasta executor_atv1
```sh
cd ./compiladores_SENAC/executor_atv1/
```
- Compilar o código:
```sh
gcc -W -Wall -pedantic -std=c99 -o simulador main.c
```
- Após isso, para finalizar o teste, executar:
```sh
./simulador <arquivo_para_teste>.mem
```
## **OBSERVAÇÃO**
Os dois arquivos criados (soma.mem e multiplicacao_entrega.mem) foram feitos no neander e exportados.

Subi apenas para demonstração. Não necessariamente precisam ser esses arquivos.

## **ASSEMBLER**

- Clonar o repositório:
```sh
git clone https://github.com/sggui/compiladores_SENAC.git
```
- Entrar na pasta assembler_atv1
```sh
cd ./compiladores_SENAC/assembler_atv1/
```
- Compilar o código:
```sh
gcc -o assembler Tokenizer.c main.c Assembler.c -Wall
```
- Após isso, para finalizar o teste, executar:
```sh
./assembler <arquivo_de_codigo>.txt <output>.mem
```
## **OBSERVAÇÃO**
O arquivo criado (entrada.txt) foi feito apenas para teste, não necessariamente precisa ser ele. Pode ser um outro TXT. Caso queira alterar a operação ou os valores que serão somados, deve-se alterar o entrada.txt com o código desejado.

Ao rodar, ele criará um .dump e substituirá dentro do output.mem o binário para upload no Neander. O .dump foi criado apenas para um debug mais fácil, não precisa ser utilizado. O arquivo que deve ser submetido ao Neander é o output.mem.

O arquivo criado (entrada.txt) foi feito apenas para teste, não necessariamente precisa ser ele. Pode ser um outro TXT. Caso queira alterar a operação ou os valores que serão somados, deve-se alterar o entrada.txt com o código desejado.

Ao rodar, ele criará um .dump e substituirá dentro do output.mem o binário para upload no Neander. O .dump foi criado apenas para um debug mais fácil, não precisa ser utilizado. O arquivo que deve ser submetido ao Neander é o output.mem.

**IMPORTANTE**

Lembre-se de verificar se o Neander está configurado para Hexadecimal, assim, os valores ficarão iguais entre o entrada.txt e o que aparecerá no Neander.

