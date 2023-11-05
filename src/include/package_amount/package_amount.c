#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../definitions/sizes.h"
#include "package_amount.h"

char *get_package_amount()
{
    FILE *package_file;
    char *package_amount;
    package_amount = malloc((PACKAGE_AMOUNT_SIZE+1)*sizeof(*package_amount));

    if (system("which dpkg > /dev/null 2>&1") == 0)
    {
        package_file = popen("dpkg -l | wc -l", "r");
    }
    else if (system("which pacman > /dev/null 2>&1") == 0)
    {
        package_file = popen("pacman -Q | wc -l", "r");
    }
    else if (system("which xbps-query > /dev/null 2>&1") == 0)
    {
        package_file = popen("xbps-query -l | wc -l", "r");
    }
    else if (system("which dnf > /dev/null 2>&1") == 0)
    {
        package_file = popen("dnf list installed | wc -l", "r");
    }
    else
    {
        fprintf(stderr, "error: unknown package manager\n");
        return NULL;
    }
    fgets(package_amount, PACKAGE_AMOUNT_SIZE, package_file);
    package_amount[strlen(package_amount) - 1] = 0;
    return package_amount;
}
