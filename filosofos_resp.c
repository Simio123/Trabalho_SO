#include <stdio.h>      // Entrada e saída padrão
#include <stdlib.h>     // Funções como malloc, free, atoi
#include <unistd.h>     // sleep()
#include <pthread.h>    // Threads POSIX
#include <time.h>       // time() e srand()

// Variáveis globais
int N_FILOS;                            // Número de filósofos
int *refeicoes;                         // Array com número de refeições por filósofo
pthread_mutex_t *hashis;               // Array de mutexes (1 por hashi)
pthread_t *filos;                      // Array de threads dos filósofos

pthread_mutex_t estat_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para estatísticas

int comendo_agora = 0;                 // Contador de quantos estão comendo atualmente
int max_comendo_simultaneamente = 0;   // Máximo de filósofos que comeram ao mesmo tempo

int max_ciclos = -1;                   // Número máximo de ciclos (refeições) por filósofo

// Simula o tempo que o filósofo passa pensando
void tempo_pensar(int id) {
    int tempo = rand() % 3 + 1; // De 1 a 3 segundos
    printf("Filosofo %d está pensando por %d segundos\n", id, tempo);
    sleep(tempo);
}

// Simula o tempo que o filósofo passa comendo e atualiza estatísticas
void tempo_comer(int id) {
    int tempo = rand() % 3 + 1;

    // Atualiza o número de filósofos comendo
    pthread_mutex_lock(&estat_mutex);
    comendo_agora++;
    if (comendo_agora > max_comendo_simultaneamente)
        max_comendo_simultaneamente = comendo_agora;
    printf("Filosofo %d começou a comer (%d comendo agora)\n", id, comendo_agora);
    pthread_mutex_unlock(&estat_mutex);

    sleep(tempo); // Simula o tempo comendo

    // Atualiza o número de filósofos comendo
    pthread_mutex_lock(&estat_mutex);
    comendo_agora--;
    printf("Filosofo %d terminou de comer (%d comendo agora)\n", id, comendo_agora);
    pthread_mutex_unlock(&estat_mutex);
}

// Filósofo tenta pegar um hashi (mutex)
void pegar(int id, int hashi) {
    pthread_mutex_lock(&hashis[hashi]);
    printf("Filosofo %d pegou o hashi %d\n", id, hashi);
}

// Filósofo libera um hashi (mutex)
void liberar(int id, int hashi) {
    pthread_mutex_unlock(&hashis[hashi]);
    printf("Filosofo %d liberou o hashi %d\n", id, hashi);
}

// Função executada por cada thread de filósofo
void *filosofo(void *arg) {
    long i = (long)arg;
    int c1, c2;
    int ciclos = 0;

    while (max_ciclos == -1 || ciclos < max_ciclos) {
        tempo_pensar(i); // Filósofo pensa

        // Para evitar deadlock: define a ordem de pegar os hashis
        if (i % 2 == 0) {
            c1 = i;
            c2 = (i + 1) % N_FILOS;
        } else {
            c1 = (i + 1) % N_FILOS;
            c2 = i;
        }

        pegar(i, c1); // Pega primeiro hashi
        pegar(i, c2); // Pega segundo hashi

        tempo_comer(i); // Come
        refeicoes[i]++; // Registra refeição
        ciclos++;       // Incrementa ciclo

        liberar(i, c1); // Solta primeiro hashi
        liberar(i, c2); // Solta segundo hashi
    }

    return NULL;
}

// Função principal
int main(int argc, char *argv[]) {
    // Verifica se os argumentos foram fornecidos
    if (argc != 3) {
        printf("Uso: %s <numero_de_filosofos> <numero_de_ciclos>\n", argv[0]);
        return 1;
    }

    // Lê número de filósofos e ciclos
    N_FILOS = atoi(argv[1]);
    max_ciclos = atoi(argv[2]);

    srand(time(NULL)); // Inicializa gerador de números aleatórios

    // Aloca memória para arrays
    refeicoes = calloc(N_FILOS, sizeof(int));
    hashis = malloc(N_FILOS * sizeof(pthread_mutex_t));
    filos = malloc(N_FILOS * sizeof(pthread_t));

    // Inicializa mutex de cada hashi
    for (int i = 0; i < N_FILOS; i++) {
        pthread_mutex_init(&hashis[i], NULL);
    }

    // Cria as threads dos filósofos
    for (long i = 0; i < N_FILOS; i++) {
        pthread_create(&filos[i], NULL, filosofo, (void *)i);
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < N_FILOS; i++) {
        pthread_join(filos[i], NULL);
    }

    // Imprime estatísticas finais
    printf("\n=== Estatísticas finais ===\n");
    for (int i = 0; i < N_FILOS; i++) {
        printf("Filosofo %d comeu %d vezes\n", i, refeicoes[i]);
    }
    printf("Máximo de filósofos comendo simultaneamente: %d\n", max_comendo_simultaneamente);

    // Libera recursos e destrói mutexes
    for (int i = 0; i < N_FILOS; i++) {
        pthread_mutex_destroy(&hashis[i]);
    }
    pthread_mutex_destroy(&estat_mutex);
    free(refeicoes);
    free(hashis);
    free(filos);

    return 0;
}
