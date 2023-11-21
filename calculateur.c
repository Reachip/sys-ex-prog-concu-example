#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define N 400

typedef struct Calcul
{
    int nb1;
    int nb2;
    char operation[2]; // Changé à 2 pour inclure le caractère de fin de chaîne
} Calcul;

int main()
{
    int pipes[N][2];
    pid_t pid;
    int status;

    // Initialiser le générateur de nombres aléatoires

    for (int client = 0; client < N; client++)
    {
        if (pipe(pipes[client]) == -1)
        {
            perror("Erreur création pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int client = 0; client < N; client++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("Erreur fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) // Processus enfant
        {
            srand(time(NULL));
            Calcul calcul = {rand() % 100, rand() % 100, "+"};
            calcul.operation[1] = '\0'; // Assurez-vous que la chaîne est correctement terminée

            close(pipes[client][0]);
            if (write(pipes[client][1], &calcul, sizeof(calcul)) == -1)
            {
                perror("Erreur écriture pipe");
                exit(EXIT_FAILURE);
            }
            close(pipes[client][1]);

            printf("Send : %d %s %d\n", calcul.nb1, calcul.operation, calcul.nb2);
            exit(EXIT_SUCCESS); // Terminez chaque processus enfant après l'envoi
        }
    }

    // Processus parent
    for (int client = 0; client < N; client++)
    {
        Calcul receivedCalcul;

        close(pipes[client][1]);
        if (read(pipes[client][0], &receivedCalcul, sizeof(receivedCalcul)) == -1)
        {
            perror("Erreur lecture pipe");
            exit(EXIT_FAILURE);
        }
        close(pipes[client][0]);

        if (receivedCalcul.operation[0] == '+')
        {
            int result = receivedCalcul.nb1 + receivedCalcul.nb2;
            printf("Receive : %d %s %d = %d\n", receivedCalcul.nb1, receivedCalcul.operation, receivedCalcul.nb2, result);
        }
    }

    // Attendre que tous les processus enfants se terminent
    while ((pid = wait(&status)) > 0)
        ;

    return 0;
}
