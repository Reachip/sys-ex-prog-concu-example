#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

void *task1(void* arg) {
    sem_t* sem = arg;
    puts("start task1");
    sleep(5);
    puts("wake task2");
    sem_post(sem);
    return NULL;
}
void *task2(void* arg) {
    sem_t* sem = arg;
    puts("start task2");
    sem_wait(sem);
    puts("restart task2");
    return NULL;
}

int main(void)
{
    sem_t sem;
    sem_init(&sem,0,0);
    pthread_t thread, pthread_t, thread2;
    pthread_create(&thread,NULL,task1,&sem);
    pthread_create(&thread2,NULL,task2,&sem);
    pthread_join(thread,NULL);
    pthread_join(thread2,NULL);
    sem_destroy(&sem);
    return 0;
}