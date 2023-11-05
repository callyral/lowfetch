#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_read.h"

char *file_read(const char *filename, size_t size)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "error: '%s' is inaccessible or does not exist\n", filename);
        return NULL;
    }

    char *output;
    output = malloc((size+1)*sizeof(*output)); 

    fread(output, size, 1, file);   // store the file into output
    output[strlen(output) - 1] = 0; // trim the trailing newline

    fclose(file);                   // close the file as it's already been stored

    return output;
}
