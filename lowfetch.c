#include "lowfetch.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ASCII_FILE_SIZE 4096
#define DEFAULT_SIZE 256

/* from https://www.asciiart.eu/animals/cats */
static char *ascii_default = " |\\'/-..--.\n"
                             " / _ _   ,  ;\n"
                             "`~=`Y'~_<._./\n"
                             " <`-....__.'";

enum ColorChars
{
  WHITE = 'w',
  RED = 'r',
  GREEN = 'g',
  YELLOW = 'y',
  BLUE = 'b',
  MAGENTA = 'm',
  CYAN = 'c'
};

enum FileMode
{
    MODE_FILE_NONE,
    MODE_FILE
};

struct SystemInfo
{
    char *ascii;
    char *distro_id;
    char *uptime;
    char *kernel_version;
};

int main(int argc, char **argv)
{
    enum ColorChars accent_color = WHITE;
    enum FileMode file_mode = MODE_FILE_NONE;
    
    char *ascii_filename;
    /* use XDG_CONFIG_HOME if it exists, otherwise default to ~/.config/ */
    if (getenv("XDG_CONFIG_HOME"))
    {
        ascii_filename = strcat(getenv("XDG_CONFIG_HOME"), "/lowfetch/ascii"); 
    }
    else
    {
        ascii_filename = strcat(getenv("HOME"), "/.config/lowfetch/ascii"); 
    }
    
    if (argc > 1)
    {
        int i;
        for (i = 1; i < argc; ++i)
        {
            /* pretty terrible argument parsing, fix eventually */
            if (strcmp(argv[i], "--file") == 0)
            {
                file_mode = MODE_FILE;
                /* set ascii_filename to what is after "--file", unless it starts with '-' */
                if (argv[i+1] && argv[i+1][0] != '-')
                {
                    ascii_filename = argv[i+1];
                }
                continue;
            }
            if (strcmp(argv[i], "--color") == 0)
            {
                accent_color = BLUE;
                if (argv[i+1] && argv[i+1][0] != '-')
                {
                    accent_color = argv[i+1][0];
                }

                continue;
            }
        }
    }

    char *ascii = get_ascii(file_mode, ascii_filename, ASCII_FILE_SIZE);
    char *distro_id = get_distro_id(DEFAULT_SIZE);
    char *uptime = get_uptime(DEFAULT_SIZE);
    char *kernel_version = get_kernel_version(DEFAULT_SIZE);

    struct SystemInfo system_info = {ascii, distro_id, uptime, kernel_version};

    info_print(accent_color, system_info);
    
    free(ascii);
    //free(distro_id);
    free(uptime);
    free(kernel_version);
    return 0;
}

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

    fread(output, size, 1, file); // store the file into output
    if (!output) 
    {
        fprintf(stderr, "error: unable to read '%s'\n", filename);
        return NULL;
    }
    else
    {
        output[strlen(output) - 1] = 0; // trim the trailing newline
    }
    fclose(file); // close the file since it's already been stored

    return output;
}

char *get_ascii(enum FileMode file_mode, const char *filename, size_t size)
{
    char *ascii;
    switch (file_mode)
    {
        case MODE_FILE_NONE:
            ascii = malloc((size+1) * sizeof(*ascii));
            strcpy(ascii, ascii_default); // copy ascii_default into ascii
            return ascii;
        case MODE_FILE:
            ascii = file_read(filename, size);
            /* if ascii is NULL, use ascii_default instead */
            if (!ascii)
            {
                ascii = malloc((size+1) * sizeof(*ascii));
                strcpy(ascii, ascii_default);
                return ascii;
            }
            else
            {
                return ascii;
            }
    }
}

char *get_distro_id(size_t size)
{
    /* TODO: parser for /etc/os-release */
    //char *filename = "/etc/os-release";
    return "work in progress!!!";
}

char *get_uptime(size_t size)
{
    char *filename = "/proc/uptime";
    char *uptime = file_read(filename, size);
    return uptime;
}

char *get_kernel_version(size_t size)
{
    char *filename = "/proc/version";
    char *kernel_version = file_read(filename, size);
    return kernel_version;
}

char *get_ansi_color_from(enum ColorChars color)
{
    switch (color)
    {
        case WHITE:
            return ANSI_COLOR_RESET;
        case RED:
            return ANSI_COLOR_RED;
        case GREEN:
            return ANSI_COLOR_GREEN;
        case YELLOW:
            return ANSI_COLOR_YELLOW;
        case MAGENTA:
            return ANSI_COLOR_MAGENTA;
        case CYAN:
            return ANSI_COLOR_CYAN;
        default:
            return ANSI_COLOR_BLUE;
    }
}

int info_print(enum ColorChars accent_color, struct SystemInfo system_info)
{
    /* TODO: add bold options */
    printf("%s%s%s\n", get_ansi_color_from(accent_color), system_info.ascii, ANSI_COLOR_RESET);
    printf("%sdistro:%s %s\n", get_ansi_color_from(accent_color), ANSI_COLOR_RESET, system_info.distro_id);
    printf("%suptime:%s %s\n", get_ansi_color_from(accent_color), ANSI_COLOR_RESET, system_info.uptime);
    printf("%skernel:%s %s\n", get_ansi_color_from(accent_color), ANSI_COLOR_RESET, system_info.kernel_version);

    return 0;
}
