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

// ----------- DECLARACIÓN DE FUNCIONES -----------

// Inicializa los ficheros de los hijos. Sigue el siguiente formato:
// Letra_LETRAHIJO.txt
// Siendo LETRAHIJO la letra, en minúscula, asociada al hijo. Dicha
// letra viene dada por el parámetro name
void crear_fichero_vacio(const char *name);

// Genera el string que se pasará por parámetro a la función
// crear_fichero_vacio.
void inicializar_fichero(char letra);

// Función que guarda las palabras que comienzan por la letra asociada a un
// hijo en el fichero que le corresponda. Las palabras son tomadas del
// fichero rae.txt
void separar_por_letras();

// Función que reemplaza un substring por otro en un texto dado.
// Parámetros:
// string_original: string base
// substring_original: substring que queremos que sea sustituido y se encuentra en string_original
// substring_nuevo: substring que queremos que aparezca en string_original donde estuviera substring_original
void reemplazasubstring_nuevo(char *, char *, char *);

// Función que muestra el mensaje que un hijo ha enviado a otro hijo para indicar
// que ha finalizado.
void mensaje_finalizar();
// ------------------------------------------------

// -------------- VARIABLES GLOBALES --------------
char LETRAS_ABECEDARIO[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'}; // Array de char con todas las letras del abecedario.
int tuberia[2];                                                                                                                                                // Tubería con la que se comunicarán padres e hijos.

// Zona de memoria compartida.
typedef struct
{
    int pid;    // Identificador de proceso.
    int salida; // (0 = el proceso no ha terminado todavía) (1 = el proceso ha terminado)
    char letra; // Letra asociada al proceso.
} datos;

datos *puntero; // Puntero a la zona de memoria compartida.
// ------------------------------------------------

int main(int argc, char const *argv[])
{

    // Comprobamos si el fichero rae.txt existe o no.
    FILE *file;
    if (file = fopen("rae.txt", "r"))
    {
        // Existe.
        fclose(file);

        // Variables de las zonas de memoria y sobre los procesos.
        int estado;
        int id;
        int pid;
        key_t clave;
        clave = ftok("/bin/ls", 11);
        id = shmget(clave, sizeof(LETRAS_ABECEDARIO) * sizeof(datos), 0777 | IPC_CREAT); // El dos indica el número de campos del struct
        puntero = (datos *)shmat(id, (char *)0, 0);

        printf("Comenzamos el programa.\n");

        // Iniciamos la zona de memoria.
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

        pipe(tuberia); // Creamos la tubería.

        for (int i = 0; i < sizeof(LETRAS_ABECEDARIO); i++)
        {
            pid = fork();
            if (pid == 0)
            {
                // ------------------- HIJO --------------------

                // Armado de señal.
                // Si el hijo recibe la señal SIGUSR1, el hijo escribe en su fichero las palabras que empiecen
                // por su letra asociada.
                struct sigaction senal_separar;
                senal_separar.sa_handler = separar_por_letras;
                sigemptyset(&senal_separar.sa_mask);
                senal_separar.sa_flags = 0;
                sigaction(SIGUSR1, &senal_separar, NULL);
                puntero[i].pid = getpid();

                // Armado de señal.
                // Si el hijo recibe la señal SIGUSR2, el hijo muestra un mensaje indicando que el otro hijo
                // que ha enviado la señal ha finalizado.
                struct sigaction senal_finalizar_comunicar;
                senal_finalizar_comunicar.sa_handler = mensaje_finalizar;
                sigemptyset(&senal_finalizar_comunicar.sa_mask);
                senal_finalizar_comunicar.sa_flags = 0;
                sigaction(SIGUSR2, &senal_finalizar_comunicar, NULL);
                puntero[i].pid = getpid();

                // Mientras el hijo no haya realizado su tarea, salida = 0
                while (puntero[i].salida == 0)
                {
                    printf("Esperando a que mi padre me diga que finalice %d \n", getpid());
                    usleep(150);
                }

                // El hijo ya ha realizado su tarea. Va a enviarse a los todos los hijos que no han acabado
                // (excepto a él mismo) un mensaje indicando que ya ha acabado de escribir las palabras.
                // Envía su propio PID.
                for (int i = 0; i < sizeof(LETRAS_ABECEDARIO); i++)
                {
                    if (puntero[i].pid != getpid() && puntero[i].salida == 0)
                    {
                        int pid_hijo_finalizar = getpid();
                        write(tuberia[1], &pid_hijo_finalizar, sizeof(pid_hijo_finalizar));
                        kill(puntero[i].pid, SIGUSR2);
                    }
                }

                printf("Soy el hijo %d y voy a finalizar mi ejecución. \n", getpid());
                exit(0);
            }
        }

        // ------------------- PADRE -------------------

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
        }

        // Esperamos a que todos los hijos hayan finalizado.
        for (int i = 0; i < sizeof(LETRAS_ABECEDARIO); i++)
        {
            wait(&estado);
        }

        // Mostramos todos los ficheros que han creado los hijos.
        printf("Archivos creados por los hijos: \n");
        system("ls letras");
    }
    else
    {
        // No existe.
        printf("El fichero que contiene el diccionario de la RAE no existe. Por favor, introdúzcalo con el nombre rae.txt\n");
    }

    return 0;
}

// ------------------- FUNCIONES ------------------

// Inicializa los ficheros de los hijos. Sigue el siguiente formato:
// Letra_LETRAHIJO.txt
// Siendo LETRAHIJO la letra, en minúscula, asociada al hijo. Dicha
// letra viene dada por el parámetro name
void crear_fichero_vacio(const char *name)
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

    // Hacemos que el fichero esté vacío. Esto sirve para que,
    // en caso de que existiera, no se escriba en la última línea del fichero.
    char comando[] = "cat /dev/null > letras/Letra_";
    strcat(comando, name);
    strcat(comando, extension);
    system(comando);
}

// Genera el string que se pasará por parámetro a la función
// crear_fichero_vacio.
void inicializar_fichero(char letra)
{
    char str[2];

    str[0] = letra;
    str[1] = '\0';

    crear_fichero_vacio(str);
}

// Función que guarda las palabras que comienzan por la letra asociada a un
// hijo en el fichero que le corresponda. Las palabras son tomadas del
// fichero rae.txt
void separar_por_letras()
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
            char palabra[bufferLength];

            filePointer = fopen("rae.txt", "r");

            while (fgets(palabra, bufferLength, filePointer))
            {
                // Reemplazamos todas las vocales no comunes
                // por sus equivalentes "normales" en minúscula.

                reemplazasubstring_nuevo(palabra, "á", "a");
                reemplazasubstring_nuevo(palabra, "é", "e");
                reemplazasubstring_nuevo(palabra, "í", "i");
                reemplazasubstring_nuevo(palabra, "ó", "o");
                reemplazasubstring_nuevo(palabra, "ú", "u");

                reemplazasubstring_nuevo(palabra, "Á", "a");
                reemplazasubstring_nuevo(palabra, "É", "e");
                reemplazasubstring_nuevo(palabra, "Í", "i");
                reemplazasubstring_nuevo(palabra, "Ó", "o");
                reemplazasubstring_nuevo(palabra, "Ú", "u");

                reemplazasubstring_nuevo(palabra, "ä", "a");
                reemplazasubstring_nuevo(palabra, "ë", "e");
                reemplazasubstring_nuevo(palabra, "ï", "i");
                reemplazasubstring_nuevo(palabra, "ö", "o");
                reemplazasubstring_nuevo(palabra, "ü", "u");

                reemplazasubstring_nuevo(palabra, "Ä", "a");
                reemplazasubstring_nuevo(palabra, "Ë", "e");
                reemplazasubstring_nuevo(palabra, "Ï", "i");
                reemplazasubstring_nuevo(palabra, "Ö", "o");
                reemplazasubstring_nuevo(palabra, "Ü", "u");

                // Ponemos todas las consonantes en minúscula.
                for (int j = 0; palabra[j]; j++)
                {
                    palabra[j] = tolower(palabra[j]);
                }

                int enc = 0;
                for (int w = 0; w < sizeof(palabra) && enc == 0; w++)
                {
                    for (int z = 0; z < sizeof(LETRAS_ABECEDARIO); z++)
                    {
                        // Comprobamos que la letra actual pertenece al abecedario.
                        // Si no pertenece, pasamos a la siguiente.
                        if (palabra[w] == LETRAS_ABECEDARIO[z])
                        {
                            enc = 1;
                            // Si la letra coincide con la del hijo, escribe
                            // la palabra en el fichero. Si no, pasa a la siguiente palabra.
                            if (palabra[w] == puntero[i].letra)
                            {
                                FILE *fichero_letra;
                                char nombre_fichero[200];

                                sprintf(nombre_fichero, "letras/Letra_%c.txt", puntero[i].letra);
                                fichero_letra = fopen(nombre_fichero, "a");
                                fputs(palabra, fichero_letra);
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

// Función que reemplaza un substring por otro en un texto dado.
// Parámetros:
// string_original: string base
// substring_original: substring que queremos que sea sustituido y se encuentra en string_original
// substring_nuevo: substring que queremos que aparezca en string_original donde estuviera substring_original
void reemplazasubstring_nuevo(char *string_original, char *substring_original, char *substring_nuevo)
{
    char palabra[500];
    char *ch;

    if (!(ch = strstr(string_original, substring_original)))
        return;

    strncpy(palabra, string_original, ch - string_original);
    palabra[ch - string_original] = 0;
    sprintf(palabra + (ch - string_original), "%s%s", substring_nuevo, ch + strlen(substring_original));
    string_original[0] = 0;
    strcpy(string_original, palabra);

    return reemplazasubstring_nuevo(string_original, substring_original, substring_nuevo);
}

// Función que muestra el mensaje que un hijo ha enviado a otro hijo para indicar
// que ha finalizado.
void mensaje_finalizar()
{
    int pid_hijo_finaliza;
    read(tuberia[0], &pid_hijo_finaliza, sizeof(pid_hijo_finaliza));
    printf("Soy el proceso %d y el proceso hijo %d ya ha terminado \n", getpid(), pid_hijo_finaliza);
}

// ------------------------------------------------