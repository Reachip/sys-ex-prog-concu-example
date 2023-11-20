#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
        
int main() {
    int pipefd[2];
    pid_t pid;
    const char *msg = "Bonjour du parent!\n";
    char buffer[100];

    // Créer un pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Créer un processus enfant
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Processus enfant
        close(pipefd[1]); // Fermer l'extrémité en écriture inutilisée

        // Rediriger l'entrée standard du processus enfant vers le pipe
        dup2(pipefd[0], STDIN_FILENO);

        // Lire du pipe et afficher à l'écran
        while (read(STDIN_FILENO, buffer, sizeof(buffer)) > 0) {
            write(STDOUT_FILENO, buffer, strlen(buffer));
        }

        close(pipefd[0]);
        _exit(EXIT_SUCCESS);
    } else {
        // Processus parent
        close(pipefd[0]); // Fermer l'extrémité en lecture inutilisée

        // Écrire dans le pipe
        write(pipefd[1], msg, strlen(msg));

        close(pipefd[1]); // Fermer l'extrémité en écriture après utilisation
        wait(NULL); // Attendre la fin du processus enfant
    }

    return 0;
}
