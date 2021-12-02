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
#include <string.h>

char VOCALES_MIN[5] = {'a', 'e', 'i', 'o', 'u'};
char VOCALES_MAY[5] = {'A', 'E', 'I', 'O', 'U'};
char VOCALES_MIN_TILDE[5] = {'á', 'é', 'í', 'ó', 'ú'};
char VOCALES_MAY_TILDE[5] = {'Á', 'É', 'Í', 'Ó', 'Ú'};
char VOCALES_MIN_DIERESIS[5] = {'ä', 'ë', 'ï', 'ö', 'ü'};
char VOCALES_MAY_DIERESIS[5] = {'Ä', 'Ë', 'Ï', 'Ö', 'Ü'};

char *normalizar_caracteres(char *str);

int main()
{
    char str[1] = 'Á';
    char *str_final = normalizar_caracteres(str);

    printf("Final: %s", str_final);
    return 0;
}

char *normalizar_caracteres(char *str)
{
    for (int i = 0; i <= strlen(str); i++)
    {
        for (int j = 0; j < 5; j++)
        {

            if (str[i] == VOCALES_MAY[j])
            {
                str[i] = VOCALES_MIN[j];
            }

            if (str[i] == VOCALES_MIN_TILDE[j])
            {
                str[i] = VOCALES_MIN[j];
            }

            if (str[i] == VOCALES_MAY_TILDE[j])
            {
                str[i] = VOCALES_MIN[j];
            }

            if (str[i] == VOCALES_MIN_DIERESIS[j])
            {
                str[i] = VOCALES_MIN[j];
            }

            if (str[i] == VOCALES_MAY_DIERESIS[j])
            {
                str[i] = VOCALES_MIN[j];
            }
        }
    }
}