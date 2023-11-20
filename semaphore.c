#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "dijkstra/dijkstra.c"

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

void producer(int semid_full, int semid_empty) {
    for (int i = 0; i < 20; i++) {
        P(semid_empty);
        buffer[in] = i;
        printf("Produit: %d\n", i);
        in = (in + 1) % BUFFER_SIZE;
        V(semid_full);
        sleep(1);
    }
}

void consumer(int semid_full, int semid_empty) {
    for (int i = 0; i < 20; i++) {
        P(semid_full); // Attendre qu'un élément soit disponible
        int item = buffer[out];
        printf("Consommé: %d\n", item);
        out = (out + 1) % BUFFER_SIZE;
        V(semid_empty); // Signaler qu'il y a de la place
        sleep(1);
    }
}

int main() {
    key_t key_full = 1234;
    key_t key_empty = 5678;
    int semid_full = sem_create(key_full, 0); // Nombre d'éléments dans le buffer
    int semid_empty = sem_create(key_empty, BUFFER_SIZE); // Espace disponible dans le buffer

    pid_t pid = fork();
    if (pid == 0) {
        // Processus enfant: Consommateur
        consumer(semid_full, semid_empty);
        exit(0);
    } else if (pid > 0) {
        // Processus parent: Producteur
        producer(semid_full, semid_empty);
        wait(NULL); // Attendre que le processus enfant se termine
    } else {
        perror("fork failed");
        exit(1);
    }

    sem_delete(semid_full);
    sem_delete(semid_empty);

    return 0;
}
