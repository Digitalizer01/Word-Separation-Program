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

void make_file(const char *name);
void inicializar_ficheros();

char LETRAS_ABECEDARIO[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
int tuberia[2];

typedef struct
{
    int pid;
    int salida;
    char letra;
} datos;

datos *puntero;

int main(int argc, char const *argv[])
{
    int estado;
    int id;
    int pid;
    key_t clave;
    clave = ftok("/bin/ls", 11);
    id = shmget(clave, sizeof(LETRAS_ABECEDARIO) - 1 * sizeof(datos), 0777 | IPC_CREAT); // El dos indica el número de campos del struct
    puntero = (datos *)shmat(id, (char *)0, 0);

    for (int i = 0; i < sizeof(LETRAS_ABECEDARIO); i++)
    {
        puntero[i].pid = 0;
        puntero[i].salida = 0;
        puntero[i].letra = LETRAS_ABECEDARIO[i];
        printf("-----\n");
        printf("puntero[%d].pid=%d \n", i, puntero[i].pid);
        printf("puntero[%d].salida=%d \n", i, puntero[i].salida);
        printf("puntero[%d].letra=%c \n", i, puntero[i].letra);
        printf("-----\n");
    }

    inicializar_ficheros();
    return 0;
}

void make_file(const char *name)
{
    // Creamos el directorio letras
    char dest[] = "mkdir -p letras";
    system(dest);

    // Creamos el fichero correspondiente a la letra
    char nombre_fichero[] = "touch letras/Letra_";
    strcat(nombre_fichero, name);
    system(nombre_fichero);
}

void inicializar_ficheros()
{
    char str[sizeof(LETRAS_ABECEDARIO)];
    for (size_t i = 0; i < sizeof(LETRAS_ABECEDARIO); i++)
    {
        // string is a character array
        str[0] = LETRAS_ABECEDARIO[i];
        str[1] = '\0';
        //string always ends with a null character

        //displaying the string
        make_file(str);
    }
}