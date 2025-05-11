#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <locale.h>

int readcount = 0;
int writecount = 0;
int shared = 0;

// semáforos
sem_t mutex_rc;
sem_t mutex_wc;
sem_t mutex_fila;
sem_t wrt;

// funções utilitárias
int gera_rand(int limit) {
    return rand() % limit;
}

// Funções auxiliares para ilustrar
void ler_base_de_dados() {
    int tempo_de_leitura = rand() % 5 + 1;
    printf("\t-> LENDO o banco de dados por %d segundos...\n", tempo_de_leitura);
    sleep(tempo_de_leitura);
}

void usar_banco_de_dados() {
    int tempo_de_uso = rand() % 15 + 1;
    printf("\t-> Usando conhecimento adquirido por %d segundos...\n", tempo_de_uso);
    sleep(tempo_de_uso);
}

void pensando_nos_dados() {
    int tempo_para_pensar = rand() % 10 + 1;
    printf("\t-> Pensando nos dados por %d segundos...\n", tempo_para_pensar);
    sleep(tempo_para_pensar);
}

void escrever_no_banco_de_dados() {
    int tempo_de_escrita = rand() % 10 + 1;
    printf("\t-> Escrevendo no banco de dados por %d segundos...\n", tempo_de_escrita);
    sleep(tempo_de_escrita);
}

// Funções das threads
void *leitor(void *id) {
    usleep(gera_rand(1000000));
    long i = (long)id;

    pensando_nos_dados();

    sem_wait(&mutex_fila);
    sem_wait(&mutex_rc);
    readcount++;
    if (readcount == 1) {
        sem_wait(&wrt);
    }
    sem_post(&mutex_rc);
    sem_post(&mutex_fila);

    printf("> Leitor %ld lendo\n", i);
    ler_base_de_dados();

    sem_wait(&mutex_rc);
    readcount--;
    if (readcount == 0) {
        sem_post(&wrt);
    }
    sem_post(&mutex_rc);

    usar_banco_de_dados();
    printf("< Leitor %ld terminou\n", i);
    pthread_exit(NULL);
}

void *escritor(void *id) {
    usleep(gera_rand(1000000));
    long i = (long)id;

    pensando_nos_dados();

    sem_wait(&mutex_wc);
    writecount++;
    if (writecount == 1) {
        sem_wait(&mutex_fila);
    }
    sem_post(&mutex_wc);

    sem_wait(&wrt);
    printf("+ Escritor %ld escrevendo\n", i);
    escrever_no_banco_de_dados();

    int rnd = gera_rand(100);
    shared = rnd;
    printf("\t+ Escritor %ld escreveu valor %d\n", i, rnd);
    sem_post(&wrt);

    sem_wait(&mutex_wc);
    writecount--;
    if (writecount == 0) {
        sem_post(&mutex_fila);
    }
    sem_post(&mutex_wc);

    printf("+ Escritor %ld terminou\n", i);
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    if (argc < 3) {
        printf("Uso: %s num_leitores num_escritores\n", argv[0]);
        return 1;
    }

    int N_LEITORES = atoi(argv[1]);
    int N_ESCRITORES = atoi(argv[2]);

    pthread_t *tl = malloc(N_LEITORES * sizeof(pthread_t));
    pthread_t *te = malloc(N_ESCRITORES * sizeof(pthread_t));

    sem_init(&mutex_rc, 0, 1);
    sem_init(&mutex_wc, 0, 1);
    sem_init(&mutex_fila, 0, 1);
    sem_init(&wrt, 0, 1);

    for (long i = 0; i < N_LEITORES; i++) {
        pthread_create(&tl[i], NULL, leitor, (void *)i);
    }
    for (long i = 0; i < N_ESCRITORES; i++) {
        pthread_create(&te[i], NULL, escritor, (void *)i);
    }

    for (int i = 0; i < N_LEITORES; i++) {
        pthread_join(tl[i], NULL);
    }
    for (int i = 0; i < N_ESCRITORES; i++) {
        pthread_join(te[i], NULL);
    }

    sem_destroy(&mutex_rc);
    sem_destroy(&mutex_wc);
    sem_destroy(&mutex_fila);
    sem_destroy(&wrt);

    free(tl);
    free(te);

    printf("Programa finalizado com sucesso.\n");
    return 0;
}
