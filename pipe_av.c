#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    int pipe1[2];
    int pipe2[2];
    pid_t pid1, pid2;

    if (pipe(pipe1) == -1)
    {
        perror("pipe1");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();

    if (pid1 < 0)
    {
        perror("fork1");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0)
    {                    // Premier enfant (cut)
        close(pipe1[0]); // Ferme la lecture

        int fd = open("/etc/passwd", O_RDONLY);
        if (fd == -1)
        {
            perror("open /etc/passwd");
            exit(EXIT_FAILURE);
        }

        dup2(fd, STDIN_FILENO);
        close(fd);

        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[1]);

        execlp("cut", "cut", "-f", "1,3", "-d", ":", NULL);
        perror("execlp cut");
        exit(EXIT_FAILURE);
    }

    // Suite dans le processus parent
    close(pipe1[1]); // Ferme l'écriture du premier pipe

    if (pipe(pipe2) == -1)
    {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    pid2 = fork();

    if (pid2 < 0)
    {
        perror("fork2");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0)
    {                    // Deuxième enfant (sed)
        close(pipe2[1]); // Ferme l'écriture
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[0]);

        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe2[1]);

        execlp("sed", "sed", "s+^\\(.*\\):\\(.*\\)+\\2:\\1+", NULL);
        perror("execlp sed");
        exit(EXIT_FAILURE);
    }

    // Suite dans le processus parent
    close(pipe1[0]); // Ferme la lecture du premier pipe
    close(pipe2[1]); // Ferme l'écriture du deuxième pipe

    int fd = open("users", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open users");
        exit(EXIT_FAILURE);
    }

    dup2(pipe2[0], STDIN_FILENO);
    close(pipe2[0]);

    dup2(fd, STDOUT_FILENO);
    close(fd);

    execlp("sort", "sort", "-n", NULL);
    perror("execlp sort");
    exit(EXIT_FAILURE);
}
