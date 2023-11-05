#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../definitions/sizes.h"
#include "distro.h"

char *get_distro_id()
{
    FILE *file = fopen("/etc/os-release", "r");
    if (!file)
    {
        fprintf(stderr, "error: '/etc/os-release' (distro information) is unreadable or doesn't exist\n");
        return NULL;
    }

    char *distro_id;
    distro_id = malloc((DISTRO_ID_SIZE+1)*sizeof(*distro_id));

    fgets(distro_id, DISTRO_ID_SIZE, file); // get the first line (NAME="Distro")
    fclose(file);

    distro_id[strlen(distro_id) - 1] = 0; // trim the trailing newline

    char *distro_id_token = strtok(distro_id, "=");

    // get what is after the "="
    //  if it's NAME="Distro", you'd get "Distro" with the quotes
    int i = 0;
    while (distro_id_token != NULL && i < 2)
    {
        strcpy(distro_id, distro_id_token);
        distro_id_token = strtok(NULL, "=");
        ++i;
    }
    // remove quotes
    //  setting distro[0] to 0 wouldn't work since it'd remove the pointer
    memmove(distro_id, distro_id+1, strlen(distro_id)); // first quote
    distro_id[strlen(distro_id) - 1] = 0;               // end quote

    return distro_id;
}
