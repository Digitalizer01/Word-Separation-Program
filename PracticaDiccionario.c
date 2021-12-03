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
#include <ctype.h>

void make_file(const char *name);
void inicializar_fichero(char letra);
void leersalida();
void sacarPalabras(char letra);
void replace(char *, char *, char *);

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
    char extension[] = ".txt";
    strcat(nombre_fichero, name);
    strcat(nombre_fichero, extension);
    system(nombre_fichero);

    char comando[] = "cat /dev/null > letras/Letra_";
    strcat(comando, name);
    strcat(comando, extension);
    system(comando);
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
            char cadena[20];
            sprintf(cadena, "%s/%c.txt", "Letra_", puntero[i].letra);

            FILE *filePointer;
            int bufferLength = 255;
            char buffer[bufferLength];

            filePointer = fopen("rae.txt", "r");

            while (fgets(buffer, bufferLength, filePointer))
            {
                replace(buffer, "á", "a");
                replace(buffer, "é", "e");
                replace(buffer, "í", "i");
                replace(buffer, "ó", "o");
                replace(buffer, "ú", "u");

                replace(buffer, "Á", "a");
                replace(buffer, "É", "e");
                replace(buffer, "Í", "i");
                replace(buffer, "Ó", "o");
                replace(buffer, "Ú", "u");

                replace(buffer, "ä", "a");
                replace(buffer, "ë", "e");
                replace(buffer, "ï", "i");
                replace(buffer, "ö", "o");
                replace(buffer, "ü", "u");

                replace(buffer, "Ä", "a");
                replace(buffer, "Ë", "e");
                replace(buffer, "Ï", "i");
                replace(buffer, "Ö", "o");
                replace(buffer, "Ü", "u");

                for (int j = 0; buffer[j]; j++)
                {
                    buffer[j] = tolower(buffer[j]);
                }

                int enc = 0;
                for (int w = 0; w < sizeof(buffer) && enc == 0; w++)
                {
                    for (int z = 0; z < sizeof(LETRAS_ABECEDARIO); z++)
                    {
                        if (buffer[w] == LETRAS_ABECEDARIO[z])
                        {
                            enc = 1;
                            if (buffer[w] == puntero[i].letra)
                            {
                                FILE *fichero_letra;
                                char nombre_fichero[200];

                                sprintf(nombre_fichero, "letras/Letra_%c.txt", puntero[i].letra);
                                fichero_letra = fopen(nombre_fichero, "a");
                                fputs(buffer, fichero_letra);
                                fclose(fichero_letra);
                            }
                        }
                    }
                }
            }

            fclose(filePointer);

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

void replace(char *o_string, char *s_string, char *r_string)
{
    //a buffer variable to do all replace things
    char buffer[500];
    //to store the pointer returned from strstr
    char *ch;

    //first exit condition
    if (!(ch = strstr(o_string, s_string)))
        return;

    //copy all the content to buffer before the first occurrence of the search string
    strncpy(buffer, o_string, ch - o_string);

    //prepare the buffer for appending by adding a null to the end of it
    buffer[ch - o_string] = 0;

    //append using sprintf function
    sprintf(buffer + (ch - o_string), "%s%s", r_string, ch + strlen(s_string));

    //empty o_string for copying
    o_string[0] = 0;
    strcpy(o_string, buffer);
    //pass recursively to replace other occurrences

    return replace(o_string, s_string, r_string);
}
