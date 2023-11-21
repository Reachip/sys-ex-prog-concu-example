#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>

#define BUFFER_SIZE 10 // Taille du buffer circulaire

// Structure pour le buffer circulaire
typedef struct
{
    int buffer[BUFFER_SIZE];
    int in;  // Pointeur d'écriture
    int out; // Pointeur de lecture
} CircularBuffer;

// Fonction principale
int main()
{
    key_t key = ftok("shmfile", 65);                                   // Générer une clé unique
    int shmid = shmget(key, sizeof(CircularBuffer), 0666 | IPC_CREAT); // Créer un segment de mémoire partagée

    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    // Attacher le segment de mémoire partagée
    CircularBuffer *circularBuffer = (CircularBuffer *)shmat(shmid, NULL, 0);
    if (circularBuffer == (void *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // Initialisation du buffer circulaire
    circularBuffer->in = 0;
    circularBuffer->out = 0;

    sem_t *mutex = sem_open("mutex", O_CREAT, 0644, 1);           // Semaphore pour contrôler l'accès au buffer
    sem_t *empty = sem_open("empty", O_CREAT, 0644, BUFFER_SIZE); // Semaphore pour les slots vides
    sem_t *full = sem_open("full", O_CREAT, 0644, 0);             // Semaphore pour les slots pleins

    // Code pour produire ou consommer des données
    // ...

    // Détacher le segment de mémoire partagée
    if (shmdt(circularBuffer) == -1)
    {
        perror("shmdt");
        exit(1);
    }

    // Suppression du segment de mémoire partagée
    shmctl(shmid, IPC_RMID, NULL);

    // Fermeture des sémaphores
    sem_close(mutex);
    sem_close(empty);
    sem_close(full);

    return 0;
}
