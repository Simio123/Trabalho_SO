# Sistema Produtor-Consumidor com Semáforos

Este trabalho implementa o problema clássico do **produtor-consumidor** utilizando **semáforos** para gerenciar o acesso concorrente a um buffer compartilhado. O código foi escrito em C e utiliza **threads** para simular múltiplos produtores e um consumidor, controlando a sincronização e exclusão mútua para evitar condições de corrida.

## Problema

O **problema do produtor-consumidor** descreve uma situação em que:
- **Produtores** geram dados (itens) e os armazenam em um buffer compartilhado.
- **Consumidores** retiram dados do buffer para processá-los.

A dificuldade surge ao garantir que os produtores não sobrescrevam dados que ainda não foram consumidos e que o consumidor não tente retirar dados de um buffer vazio. Para resolver isso, são usados semáforos para controlar a sincronização entre os produtores e o consumidor.

## O que foi feito

Este programa implementa:
- **Produtores** que geram números aleatórios e os armazenam em um buffer compartilhado.
- **Consumidor** que retira os números do buffer e os "consome" (simulado com um tempo de processamento).
- Utilização de **semáforos** para:
  - Controle de exclusão mútua (`mutex`) para garantir que apenas um produtor ou o consumidor acessem o buffer por vez.
  - Semáforo `cheio` para controlar quando há itens disponíveis no buffer para consumo.
  - Semáforo `vazio` para garantir que o produtor não escreva no buffer se ele estiver cheio.

Além disso, o programa simula o tempo de produção e consumo de valores com um intervalo aleatório de segundos.

## Compilação

Para compilar o programa, siga as instruções abaixo.

1. **Pré-requisitos**:
   - Sistema Linux com suporte a **pthread** e **semaforos POSIX**.
   - Compilador **gcc**.

2. **Comando para compilar**:

```bash
gcc -pthread produtor_consumidor.c -o produtor_consumidor
./produtor_consumidor <tamanho_do_buffer> <numero_de_produtores>
