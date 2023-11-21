#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0; // Nombre d'éléments dans le buffer
int in = 0;    // Index d'insertion
int out = 0;   // Index de retrait

pthread_mutex_t mutex;
pthread_cond_t full, empty;

void *producer(void *arg)
{
    int item;
    for (int i = 0; i < 20; i++)
    {
        item = rand(); // Génère une valeur aléatoire (simule la production d'informations)
        pthread_mutex_lock(&mutex);
        while (count == BUFFER_SIZE)
        {
            pthread_cond_wait(&empty, &mutex);
        }
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        printf("Produit : %d\n", item);
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consumer(void *arg)
{
    int item;
    for (int i = 0; i < 20; i++)
    {
        pthread_mutex_lock(&mutex);
        while (count == 0)
        {
            pthread_cond_wait(&full, &mutex);
        }
        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        printf("Consommé : %d\n", item);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    pthread_t prod_thread, cons_thread;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&full, NULL);
    pthread_cond_init(&empty, NULL);

    // Crée les threads producteurs et consommateurs
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Attends la fin des threads
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&full);
    pthread_cond_destroy(&empty);

    return 0;
}