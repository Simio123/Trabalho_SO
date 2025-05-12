#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

// Semáforos globais
sem_t mutex;
sem_t cheio;
sem_t vazio;

// ponteiro para a fila do buffer
int *buffer;

// buffer size
int N_BUFFER;

// producers num
int PROD_NUM;

// posicoes para acessar o buffer
int in = 0;
int out = 0;

// protótipos das funções
void *producer(void *);
void *consumer();
int gera_rand(int);
void print_buffer();
void produzindo_valor(int);
void consumindo_valor(int);

// Funções auxiliares para simular tempo
void produzindo_valor(int id) {
    int tempo = 2 + rand() % 8;
    printf("> Produtor %d está produzindo um valor... (%ds)\n", id, tempo);
    sleep(tempo);
}

void consumindo_valor(int valor) {
    int tempo = 2 + rand() % 8;
    printf("- Consumidor está consumindo o valor %d... (%ds)\n", valor, tempo);
    sleep(tempo);
}

int main(int argc, char **argv) {
    pthread_t tcons;
    pthread_t *nprod;
    long i;

    if (argc != 3) {
        printf("Uso: %s tamanho_do_buffer numero_de_produtores\n", argv[0]);
        return 0;
    }

    srand(time(NULL));

    N_BUFFER = atoi(argv[1]);
    PROD_NUM = atoi(argv[2]);

    // Inicialização dos semáforos
    sem_init(&mutex, 0, 1);
    sem_init(&cheio, 0, 0);
    sem_init(&vazio, 0, N_BUFFER);

    buffer = malloc(N_BUFFER * sizeof(int));

    for (i = 0; i < N_BUFFER; i++) {
        buffer[i] = -1;
    }

    nprod = malloc(PROD_NUM * sizeof(pthread_t));

    pthread_create(&tcons, NULL, consumer, NULL);

    for (i = 0; i < PROD_NUM; i++) {
        pthread_create(&nprod[i], NULL, producer, (void *)i);
    }

    // Espera as threads produtoras terminarem antes de finalizar o consumidor
    for (i = 0; i < PROD_NUM; i++) {
        pthread_join(nprod[i], NULL);
    }

    // Consumidor pode sair do loop após consumir todos os produtos
    pthread_join(tcons, NULL);

    // Destruição dos semáforos
    sem_destroy(&mutex);
    sem_destroy(&cheio);
    sem_destroy(&vazio);

    free(buffer);
    free(nprod);

    return 0;
}

void *consumer() {
    int produto;
    int i = 0;

    // Consumidor consome até que todos os produtos tenham sido consumidos
    while (i != PROD_NUM) {
        printf("- Consumidor esperando por recurso!\n");

        sem_wait(&cheio);   // Espera por um item disponível no buffer
        sem_wait(&mutex);   // Exclusão mútua ao acessar o buffer

        produto = buffer[out];
        printf("- Consumidor retirando valor da posição %d: %d\n", out, produto);

        if (produto == -1) {
            printf("==== ALERTA DO CONSUMIDOR ====\n");
            printf("Posição %d estava vazia\n", out);
            printf("==============================\n");
        }

        buffer[out] = -1;  // Remove o item consumido
        out = (out + 1) % N_BUFFER;

        sem_post(&mutex);   // Libera o acesso ao buffer
        sem_post(&vazio);   // Indica que há mais espaço no buffer

        consumindo_valor(produto);  // Simula o tempo de consumo
        print_buffer();  // Imprime o estado do buffer
        i++;
    }

    pthread_exit(NULL);
}

void *producer(void *id) {
    long i = (long)id;
    int produto;

    produzindo_valor(i);  // Simula o tempo de produção

    sem_wait(&vazio);  // Espera por espaço disponível no buffer
    sem_wait(&mutex);  // Exclusão mútua ao acessar o buffer

    produto = gera_rand(100);  // Gera um valor aleatório para ser produzido
    printf("> Produtor %ld vai gravar valor %d na posição %d\n", i, produto, in);

    // Verifica se a posição está ocupada antes de escrever
    if (buffer[in] != -1) {
        printf("==== ALERTA DO PRODUTOR %ld ====\n", i);
        printf("Posição %d ocupada com valor %d\n", in, buffer[in]);
        printf("===============================\n");
    }

    buffer[in] = produto;  // Coloca o produto no buffer
    in = (in + 1) % N_BUFFER;  // Atualiza a posição de entrada

    sem_post(&mutex);   // Libera o acesso ao buffer
    sem_post(&cheio);   // Indica que há um item disponível para consumo

    pthread_exit(NULL);
}

int gera_rand(int limit) {
    return rand() % limit;  // Gera um número aleatório entre 0 e (limit-1)
}

void print_buffer() {
    printf("\t== ESTADO DO BUFFER ==\n");
    for (int i = 0; i < N_BUFFER; i++) {
        printf("\ti: %d | valor: %d\n", i, buffer[i]);
    }
    printf("\n");
}
