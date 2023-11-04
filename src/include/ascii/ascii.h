#ifndef ASCII_H_
#define ASCII_H_
#include <stdbool.h>
#include <stddef.h>

/* from https://www.asciiart.eu/animals/cats */
static char *ascii_default = " |\\'/-..--.\n"
                             " / _ _   ,  ;\n"
                             "`~=`Y'~_<._./\n"
                             " <`-....__.'";

char *get_ascii(bool use_file, const char *filename, size_t size);

#endif
