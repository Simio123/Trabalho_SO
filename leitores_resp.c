#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <locale.h>

// Contadores globais de leitores e escritores ativos
int readcount = 0;
int writecount = 0;
int shared = 0; // Dado compartilhado que os escritores modificam

// Semáforos usados para controle de acesso
sem_t mutex_rc;     // Protege acesso ao readcount
sem_t mutex_wc;     // Protege acesso ao writecount
sem_t mutex_fila;   // Garante acesso justo (ordem de chegada)
sem_t wrt;          // Exclusão mútua para escritores

// Função utilitária para gerar número aleatório até um limite
int gera_rand(int limit) {
    return rand() % limit;
}

// Simula a ação de leitura no banco de dados
void ler_base_de_dados() {
    int tempo_de_leitura = rand() % 5 + 1;
    printf("\t-> LENDO o banco de dados por %d segundos...\n", tempo_de_leitura);
    sleep(tempo_de_leitura);
}

// Simula o uso dos dados após leitura
void usar_banco_de_dados() {
    int tempo_de_uso = rand() % 15 + 1;
    printf("\t-> Usando conhecimento adquirido por %d segundos...\n", tempo_de_uso);
    sleep(tempo_de_uso);
}

// Simula o tempo em que o leitor pensa nos dados antes de ler/escrever
void pensando_nos_dados() {
    int tempo_para_pensar = rand() % 10 + 1;
    printf("\t-> Pensando nos dados por %d segundos...\n", tempo_para_pensar);
    sleep(tempo_para_pensar);
}

// Simula a escrita no banco de dados
void escrever_no_banco_de_dados() {
    int tempo_de_escrita = rand() % 10 + 1;
    printf("\t-> Escrevendo no banco de dados por %d segundos...\n", tempo_de_escrita);
    sleep(tempo_de_escrita);
}

// Função executada pelas threads leitoras
void *leitor(void *id) {
    usleep(gera_rand(1000000)); // Espera aleatória para simular chegada irregular
    long i = (long)id;

    pensando_nos_dados();

    // Entrada na fila para garantir ordem de chegada
    sem_wait(&mutex_fila);
    sem_wait(&mutex_rc); // Protege o contador de leitores
    readcount++;
    if (readcount == 1) {
        sem_wait(&wrt); // Primeiro leitor bloqueia escritores
    }
    sem_post(&mutex_rc);
    sem_post(&mutex_fila); // Libera a fila para outros

    // Região crítica: leitura
    printf("> Leitor %ld lendo\n", i);
    ler_base_de_dados();

    // Saída da região crítica
    sem_wait(&mutex_rc);
    readcount--;
    if (readcount == 0) {
        sem_post(&wrt); // Último leitor libera escritores
    }
    sem_post(&mutex_rc);

    usar_banco_de_dados();
    printf("< Leitor %ld terminou\n", i);
    pthread_exit(NULL);
}

// Função executada pelas threads escritoras
void *escritor(void *id) {
    usleep(gera_rand(1000000)); // Espera aleatória para simular chegada irregular
    long i = (long)id;

    pensando_nos_dados();

    // Escritor entra na fila se for o primeiro
    sem_wait(&mutex_wc);
    writecount++;
    if (writecount == 1) {
        sem_wait(&mutex_fila); // Primeiro escritor bloqueia a fila
    }
    sem_post(&mutex_wc);

    // Região crítica: escrita
    sem_wait(&wrt); // Escritor precisa de exclusão mútua
    printf("+ Escritor %ld escrevendo\n", i);
    escrever_no_banco_de_dados();

    int rnd = gera_rand(100);
    shared = rnd;
    printf("\t+ Escritor %ld escreveu valor %d\n", i, rnd);
    sem_post(&wrt); // Libera acesso a outros escritores/leitores

    // Saída: atualiza contador de escritores
    sem_wait(&mutex_wc);
    writecount--;
    if (writecount == 0) {
        sem_post(&mutex_fila); // Último escritor libera a fila
    }
    sem_post(&mutex_wc);

    printf("+ Escritor %ld terminou\n", i);
    pthread_exit(NULL);
}

// Função principal
int main(int argc, char **argv) {
    setlocale(LC_ALL, "Portuguese"); // Suporte para acentuação no terminal
    srand(time(NULL)); // Inicializa semente para números aleatórios

    // Verifica se os argumentos foram passados corretamente
    if (argc < 3) {
        printf("Uso: %s num_leitores num_escritores\n", argv[0]);
        return 1;
    }

    int N_LEITORES = atoi(argv[1]);
    int N_ESCRITORES = atoi(argv[2]);

    // Aloca dinamicamente os vetores de threads
    pthread_t *tl = malloc(N_LEITORES * sizeof(pthread_t));
    pthread_t *te = malloc(N_ESCRITORES * sizeof(pthread_t));

    // Inicializa os semáforos
    sem_init(&mutex_rc, 0, 1);
    sem_init(&mutex_wc, 0, 1);
    sem_init(&mutex_fila, 0, 1);
    sem_init(&wrt, 0, 1);

    // Cria as threads leitoras
    for (long i = 0; i < N_LEITORES; i++) {
        pthread_create(&tl[i], NULL, leitor, (void *)i);
    }

    // Cria as threads escritoras
    for (long i = 0; i < N_ESCRITORES; i++) {
        pthread_create(&te[i], NULL, escritor, (void *)i);
    }

    // Aguarda o término das threads leitoras
    for (int i = 0; i < N_LEITORES; i++) {
        pthread_join(tl[i], NULL);
    }

    // Aguarda o término das threads escritoras
    for (int i = 0; i < N_ESCRITORES; i++) {
        pthread_join(te[i], NULL);
    }

    // Destroi os semáforos
    sem_destroy(&mutex_rc);
    sem_destroy(&mutex_wc);
    sem_destroy(&mutex_fila);
    sem_destroy(&wrt);

    // Libera memória alocada
    free(tl);
    free(te);

    printf("Programa finalizado com sucesso.\n");
    return 0;
}
