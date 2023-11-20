#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Processus enfant 1
        pid_t pid_child = fork();
        if (pid_child == 0) {
            // Processus enfant de l'enfant 1
            printf("Enfant de l'enfant 1: PID = %d, PPID = %d\n", getpid(), getppid());
        } else {
            wait(NULL); // Attendre le processus enfant de l'enfant 1
            printf("Enfant 1: PID = %d, PPID = %d\n", getpid(), getppid());
        }
    } else {
        pid2 = fork();
        if (pid2 == 0) {
            // Processus enfant 2
            printf("Enfant 2: PID = %d, PPID = %d\n", getpid(), getppid());
        } else {
            wait(NULL); // Attendre l'enfant 1
            wait(NULL); // Attendre l'enfant 2
            printf("Parent: PID = %d\n", getpid());
        }
    }

    return 0;
}

/*
Parent
│
├── Enfant 1
│   │
│   └── Enfant de l'enfant 1
│
└── Enfant 2
*/