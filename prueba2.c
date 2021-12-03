/**
 ****************************************************|
 * String replace Program                            |
 ****************************************************|
 * Takes three string input from the user
 * Replaces all the occurances of the second string
 * with the third string from the first string
 * @author Swashata
 */

/** Include Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Define the max char length */
#define MAX_L 4096

/** Prototypes */
void replace(char *, char *, char *);

int main(void)
{
    char o_string[] = "pokémon";

    replace(o_string, "A", "a");
    replace(o_string, "E", "e");
    replace(o_string, "I", "i");
    replace(o_string, "O", "o");
    replace(o_string, "U", "u");

    replace(o_string, "á", "a");
    replace(o_string, "é", "e");
    replace(o_string, "í", "i");
    replace(o_string, "ó", "o");
    replace(o_string, "ú", "u");

    replace(o_string, "Á", "a");
    replace(o_string, "É", "e");
    replace(o_string, "Í", "i");
    replace(o_string, "Ó", "o");
    replace(o_string, "Ú", "u");

    replace(o_string, "ä", "a");
    replace(o_string, "ë", "e");
    replace(o_string, "ï", "i");
    replace(o_string, "ö", "o");
    replace(o_string, "ü", "u");

    replace(o_string, "Ä", "a");
    replace(o_string, "Ë", "e");
    replace(o_string, "Ï", "i");
    replace(o_string, "Ö", "o");
    replace(o_string, "Ü", "u");

    printf("\n\nThe replaced string\n*************************************\n");
    puts(o_string);

    return 0;
}

/**
 * The replace function
 *
 * Searches all of the occurrences using recursion
 * and replaces with the given string
 * @param char * o_string The original string
 * @param char * s_string The string to search for
 * @param char * r_string The replace string
 * @return void The o_string passed is modified
 */
void replace(char *o_string, char *s_string, char *r_string)
{
    //a buffer variable to do all replace things
    char buffer[MAX_L];
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