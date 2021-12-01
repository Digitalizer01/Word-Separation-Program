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
void inicializar_fichero(char letra);
void leersalida();
void sacarPalabras(char letra);

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
    id = shmget(clave, sizeof(LETRAS_ABECEDARIO) * sizeof(datos), 0777 | IPC_CREAT); // El dos indica el número de campos del struct
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

    pipe(tuberia);

    for (int i = 0; i < sizeof(LETRAS_ABECEDARIO); i++)
    {
        pid = fork();
        if (pid == 0)
        {
            // Hijo

            struct sigaction a;
            a.sa_handler = leersalida;
            sigemptyset(&a.sa_mask);
            a.sa_flags = 0;
            sigaction(SIGUSR1, &a, NULL);
            puntero[i].pid = getpid();
            usleep(500);

            while (puntero[i].salida == 0)
            {
                printf("Esperando a que mi padre me diga que finalice %d \n", getpid());
                sleep(0.5);
            }
            printf("Hijo %d antes de finalizar \n", getpid());
            exit(0);
        }
    }

    // Esperamos a que todos los hijos se hayan creado.
    for (int i = 0; i < sizeof(LETRAS_ABECEDARIO); i++)
    {
        while (puntero[i].pid == 0)
        {
            usleep(200);
        }
    }

    // Enviamos a los hijos un mensaje indicando que ya pueden comenzar
    // a hacer sus correspondientes tareas.
    for (int i = 0; i < sizeof(LETRAS_ABECEDARIO); i++)
    {
        int comenzar = 1; // Si vale 1, indica al hijo que puede hacer sus tareas.
        write(tuberia[1], &comenzar, sizeof(comenzar));
        kill(puntero[i].pid, SIGUSR1);
        sleep(0.3);
    }

    return 0;
}

// -------------------------------------------------
// -------------------------------------------------
// -------------------------------------------------
// -------------------------------------------------
// -------------------------------------------------
// -------------------------------------------------
// -------------------------------------------------
// -------------------------------------------------

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

void inicializar_fichero(char letra)
{
    char str[2];

    // string is a character array
    str[0] = letra;
    str[1] = '\0';
    //string always ends with a null character

    //displaying the string
    make_file(str);
}

void leersalida()
{
    int v;
    read(tuberia[0], &v, sizeof(v));
    printf("Leido %d \n", v);
    for (int i = 0; i < sizeof(LETRAS_ABECEDARIO); i++)
    {
        if (getpid() == puntero[i].pid)
        {
            inicializar_fichero(puntero[i].letra);
            puntero[i].salida = 1;
        }
    }
}

void sacarPalabras(char letra)
{
    FILE *fp;

    fp = fopen("/tmp/test.txt", "w+");
    fprintf(fp, "This is testing for fprintf...\n");
    fputs("This is testing for fputs...\n", fp);
    fclose(fp);
}