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
- Entrar na pasta lexer_atv1
```sh
cd ./compiladores_SENAC/lexer_atv1/
```
- Compilar o código:
```sh
gcc -o assembler main.c parser.c lexer.c -Wall
```
- Após isso, para finalizar o teste, executar:
```sh
./assembler <arquivo_de_codigo>.txt <output>.mem
```
## **OBSERVAÇÃO**
O arquivo criado  (entrada.txt) foi feito apenas para teste, não necessariamente precisa ser ele. Pode ser um outro TXT.

