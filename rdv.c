#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#define NB_THREAD 5

typedef struct ThreadArg {
    sem_t key;
    int nb;
} ThreadArg;


void* rdv(void *args) {
    ThreadArg *threadArg = (ThreadArg*)args;
    sem_wait(&threadArg->key);
    threadArg->nb++;
    printf("Prise de rendez-vous n°%d ...\n", threadArg->nb);
    printf("Rendez-vous n°%d pris !\n", threadArg->nb);
    srand(time(NULL));
    sleep(rand()%((5+1)-0) + 0);
    sem_post(&threadArg->key);    

    return NULL;
}  

int main() {
    pthread_t threads[NB_THREAD];
    
    ThreadArg *args = malloc(sizeof(ThreadArg)); 
    args->nb = 0;

    sem_init(&args->key, 0, 1);

    for (int i = 0; i < NB_THREAD; i++) {
        if (pthread_create(&threads[i], NULL, rdv, args)) {
            fprintf(stderr, "Erreur lors de la création");
            return -1;
        }
    }

    for (int i = 0; i < NB_THREAD; i++) pthread_join(threads[i], NULL);

    sem_destroy(&args->key); 
    free(args);
    return 0;
}
