#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char LETRAS_ABECEDARIO[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

int main()
{
    char *foo = "-";
    int enc = 0;

    for (int w = 0; w < sizeof(foo) && enc == 0; w++)
    {
        for (int z = 0; z < sizeof(LETRAS_ABECEDARIO); z++)
        {
            if (foo[w] == LETRAS_ABECEDARIO[z])
            {
                enc = 1;
            }
        }
    }

    if (enc == 0)
    {
        printf("No se ha encontrado ninguna letra del abecedario en el string. \n");
    }
    else
    {
        printf("Se ha encontrado una letra del abecedario en el string. \n");
    }

    return (0);
}