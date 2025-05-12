# Problema dos Filósofos (Com Mutexes)

Este projeto implementa a solução do famoso problema dos filósofos usando threads e mutexes para garantir a sincronização entre os filósofos. O objetivo é simular um cenário onde vários filósofos pensam e comem alternadamente, compartilhando recursos limitados (os hashis), de maneira que evite deadlocks e condições de corrida.

## Descrição do Problema

O problema dos filósofos descreve um cenário com N filósofos sentados ao redor de uma mesa, cada um com um prato de comida. Entre cada par de filósofos há um hashi (pauzinhos para comer). Para comer, cada filósofo precisa de dois hashis, um à sua esquerda e outro à sua direita. O desafio é garantir que os filósofos possam comer de maneira segura sem bloquear uns aos outros ou entrar em um deadlock.

## Solução

A solução implementa os seguintes conceitos:

- **Mutexes**: Cada hashi é protegido por um mutex para garantir que apenas um filósofo o utilize por vez.
- **Contagem de filósofos comendo**: A solução rastreia quantos filósofos estão comendo simultaneamente e evita o deadlock, permitindo que eles peguem os hashis em uma ordem específica (para filósofos de ID par e ímpar).
- **Estatísticas**: O número de refeições feitas por cada filósofo e o número máximo de filósofos comendo simultaneamente são rastreados.

## Como Funciona

- O número de filósofos e o número de ciclos (refeições) a serem realizados são passados como argumentos de linha de comando.
- Cada filósofo é representado por uma thread, que tenta pegar os dois hashis (um à esquerda e outro à direita), comer por um tempo aleatório, e depois liberar os hashis.
- A execução é controlada para evitar deadlocks e fornecer informações sobre a quantidade de refeições feitas.

## Compilação

Para compilar o programa, utilize o comando `gcc` com suporte a threads POSIX:

```bash
gcc -pthread filosofos.c -o filosofos 
./filosofos <numero_de_filosofos> <numero_de_ciclos>

