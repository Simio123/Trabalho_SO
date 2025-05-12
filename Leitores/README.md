# Simulação de Leitores e Escritores com Controle de Acesso

Este projeto simula a situação clássica de leitores e escritores em um banco de dados compartilhado, onde múltiplos leitores podem acessar os dados ao mesmo tempo, mas apenas um escritor pode modificar os dados por vez. A implementação utiliza semáforos para gerenciar o acesso simultâneo, garantindo que leitores e escritores possam acessar o banco de dados de forma controlada e justa.

## Funcionamento

O programa cria threads para leitores e escritores. Leitores podem acessar o banco de dados simultaneamente, desde que não haja escritores ativos. Escritores, por outro lado, têm acesso exclusivo ao banco de dados, ou seja, nenhum leitor ou outro escritor pode acessar o banco enquanto um escritor está atuando.

O código é baseado na solução do problema de sincronização de leitores e escritores, utilizando semáforos para controlar o acesso à seção crítica. Além disso, há uma fila para garantir a ordem de chegada dos leitores e escritores.

## Estrutura do Código

- **Leitores:** Podem acessar os dados simultaneamente, mas se houver algum escritor ativo, todos os leitores devem esperar.
- **Escritores:** Têm acesso exclusivo aos dados e bloqueiam a leitura durante sua operação.
- **Semáforos:** São usados para controlar o acesso ao contador de leitores, o contador de escritores, a fila de entrada e a exclusão mútua para os escritores.

## Compilação

Para compilar o programa, utilize o comando `gcc` com suporte a threads POSIX:

```bash
   gcc -pthread leitores_escritores.c -o leitores_escritores
   ./leitores_escritores <numero_de_leitores> <numero_de_escritores>
