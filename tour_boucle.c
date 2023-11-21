#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define N 10

int main(int argc, char *argv[])
{
    int i;

    for (i = 0; i < N; i++)
    {
        sleep(1);
        printf("%d %s\n", i, argv[0]);
        execlp(argv[0], argv[0], NULL);
    }

    return 0;
}
