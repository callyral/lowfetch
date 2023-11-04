#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

char *get_shell()
{
    char *shell = getenv("SHELL");
    if (!shell)
    {
        fprintf(stderr, "error: $SHELL is unknown\n");
        return NULL;
    }

    return shell;
}
