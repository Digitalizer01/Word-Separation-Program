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

void make_directory(const char *name)
{
#ifdef __linux__
    mkdir(name, 777);
#else
    _mkdir(name);
#endif
}

int main(int argc, char const *argv[])
{

    char letras_abecedario[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'x', 'y', 'z'};
    char str[26];
    for (size_t i = 0; i < 26; i++)
    {
        // string is a character array
        str[0] = letras_abecedario[i];
        str[1] = '\0';
        //string always ends with a null character

        //displaying the string
        make_directory(str);
    }

    return 0;
}
