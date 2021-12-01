#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>

char LETRAS_ABECEDARIO[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

int valueinarray(char val, char arr[500]);

int main(int argc, char const *argv[])
{
    FILE *file = fopen("test.txt", "r"); /* should check the result */
    char line[500];

    while (fgets(line, sizeof(line), file))
    {
        int enc = 0;
        for (int i = 0; i < 500 && enc == 0; i++)
        {
            if (valueinarray(line[i], LETRAS_ABECEDARIO))
            {
                printf("Contiene la %c \n", line[i]);
                enc = 1;
            }
        }

        printf("%s", line);
    }

    fclose(file);

    return 0;
}

int valueinarray(char val, char arr[500])
{
    int i;
    for (i = 0; i < 500; i++)
    {
        if (arr[i] == val)
            return 1;
    }
    return 0;
}
