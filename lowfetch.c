#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "lowfetch.h"

/* modules*/
#include "include/colors.h"
#include "include/package_amount.h"
#include "include/kernel_version.h"
#include "include/lowfetch_base.h"

/* size constants/macros */
#define ASCII_FILESIZE 4096
#define KERNEL_VERSION_SIZE 512
#define FILENAME_SIZE 256
#define DISTRO_ID_SIZE 256
#define UPTIME_SIZE 256
#define ORDER_FILESIZE 128
#define PACKAGE_AMOUNT_SIZE 32

/* from https://www.asciiart.eu/animals/cats */
static char *ascii_default = " |\\'/-..--.\n"
                             " / _ _   ,  ;\n"
                             "`~=`Y'~_<._./\n"
                             " <`-....__.'";

enum SystemChars
{
    CHAR_ASCII = 'a',
    CHAR_DISTRO_ID = 'd',
    CHAR_KERNEL_VERSION = 'k',
    CHAR_PACKAGE_AMOUNT = 'p',
    CHAR_SHELL = 's',
    CHAR_UPTIME = 'u'
};

struct SystemInfo
{
    char *ascii;
    char *distro_id;
    char *kernel_version;
    char *package_amount;
    char *shell;
    char *uptime;
};

int main(int argc, char **argv)
{
    bool accent_bold = false;
    char accent_color_char = CHAR_WHITE; // white means no color
    
    bool use_ascii_file = false;
    char ascii_filename[FILENAME_SIZE];
    bool use_order_file = false;
    char order_filename[FILENAME_SIZE];
    /* use XDG_CONFIG_HOME if it exists, otherwise default to ~/.config/ */
    if (getenv("XDG_CONFIG_HOME"))
    {
        strcpy(ascii_filename, getenv("XDG_CONFIG_HOME"));
        strcat(ascii_filename, "/lowfetch/ascii");

        strcpy(order_filename, getenv("XDG_CONFIG_HOME"));
        strcat(order_filename, "/lowfetch/order"); 
    }
    else
    {
        strcpy(ascii_filename, getenv("HOME"));
        strcat(ascii_filename, "/.config/lowfetch/ascii"); 

        strcpy(order_filename, getenv("HOME"));
        strcat(order_filename, "/.config/lowfetch/order"); 
    }

    bool shorten_kernel_version = false;
    
    /* CLI argument parsing */
    /* format: { short, long, desc }*/
    const char *argdef_help[] =           {"-h",  "--help",           "show this help menu"};
    const char *argdef_ascii_file[] =     {"-a",  "--ascii",          "select ascii file"};
    const char *argdef_order_file[] =     {"-o",  "--order",          "select order file"};
    const char *argdef_color[] =          {"-c",  "--color",          "select color"};
    const char *argdef_kernel_shorten[] = {"-ks", "--kernel-shorten", "shorten kernel version"};
    const char *argdef_bold[] =           {"-b",  "--bold",           "toggle bold colors"};
    // arguments to include in help menu
    #define ARGDEFS_LIST_SIZE 6
    const char **argdefs_list[ARGDEFS_LIST_SIZE] = {argdef_help, argdef_ascii_file, argdef_order_file, argdef_kernel_shorten, argdef_color, argdef_bold};
    if (argc > 1)
    {
        for (int i = 1; i < argc; ++i)
        {
            if (arg_parse(argdef_ascii_file, argv[i]))
            {
                use_ascii_file = true;
                if (argv[i+1] && argv[i+1][0] != '-')
                {
                    strcpy(ascii_filename, argv[i+1]);
                }
                continue;
            }
            if (arg_parse(argdef_order_file, argv[i]))
            {
                use_order_file = true;
                if (argv[i+1] && argv[i+1][0] != '-')
                {
                    strcpy(order_filename, argv[i+1]);
                }
                continue;
            }
            if (arg_parse(argdef_color, argv[i]))
            {
                if (argv[i+1] && argv[i+1][0] != '-')
                {
                    accent_color_char = argv[i+1][0];
                }
                continue;
            }
            if (arg_parse(argdef_kernel_shorten, argv[i]))
            {
                shorten_kernel_version = true;
                continue;
            }
            if (arg_parse(argdef_bold, argv[i]))
            {
                accent_bold = true;
                continue;
            }
            if (arg_parse(argdef_help, argv[i]))
            {
                help_menu_print(argdefs_list);
                return 0;
            }
        }
    }

    /* requires freeing */
    char *ascii = get_ascii(use_ascii_file, ascii_filename, ASCII_FILESIZE);
    char *distro_id = get_distro_id(DISTRO_ID_SIZE);
    char *kernel_version = get_kernel_version(shorten_kernel_version, KERNEL_VERSION_SIZE);
    char *package_amount = get_package_amount(PACKAGE_AMOUNT_SIZE);
    char *uptime = get_uptime(UPTIME_SIZE);

    /* does not require freeing */
    char *shell = get_shell();

    struct SystemInfo system_info = 
    {
        .ascii = ascii,
        .distro_id = distro_id,
        .kernel_version = kernel_version,
        .package_amount = package_amount,
        .shell = shell,
        .uptime = uptime
    };

    info_print(accent_color_char, accent_bold, use_order_file, order_filename, ORDER_FILESIZE, system_info);

    free(ascii);
    free(distro_id);
    free(kernel_version);
    free(package_amount);
    free(uptime);
    return 0;
}

bool arg_parse(const char *argdef[], const char *string)
{
    return strcmp(string, argdef[0]) == 0 || strcmp(string, argdef[1]) == 0;
}

void help_menu_print(const char **argdefs_list[])
{
    printf("usage: lowfetch [options]\n");
    for(int i = 0; i < ARGDEFS_LIST_SIZE; ++i)
    {
        printf("%s, %s,\t%s\n", argdefs_list[i][0], argdefs_list[i][1], argdefs_list[i][2]);
    }
}
#undef ARGDEFS_LIST_SIZE

char *get_ascii(bool use_file, const char *filename, size_t size)
{
    char *ascii;

    if (!use_file)
    {
        ascii = malloc((size+1) * sizeof(*ascii));
        strcpy(ascii, ascii_default); // copy ascii_default into ascii
        return ascii;
    }

    if (access(filename, R_OK) == -1)
    {
        fprintf(stderr, "error: '%s' is unreadable or doesn't exist\n", filename);

        ascii = malloc((size+1) * sizeof(*ascii));
        strcpy(ascii, ascii_default);
        return ascii;
    }
    else 
    {
        ascii = file_read(filename, size);
        return ascii;
    }
}

char *get_distro_id(size_t size)
{
    FILE *file = fopen("/etc/os-release", "r");
    if (!file)
    {
        fprintf(stderr, "error: '/etc/os-release' is unreadable or doesn't exist\n");
        return NULL;
    }

    char *distro_id;
    distro_id = malloc((size+1)*sizeof(*distro_id));

    fgets(distro_id, size, file); // get the first line (NAME="Distro")
    fclose(file);

    distro_id[strlen(distro_id) - 1] = 0; // trim the trailing newline

    
    char *distro_id_token = strtok(distro_id, "=");
    
    // get what is after the "=" (if it's NAME="Distro", you get "Distro" with the quotes)
    int i = 0;
    while (distro_id_token != NULL && i < 2)
    {
        strcpy(distro_id, distro_id_token);
        distro_id_token = strtok(NULL, "=");
        ++i;
    }
    // remove first quote
    // distro[0] = 0; does not work since it would remove the pointer
    memmove(distro_id, distro_id+1, strlen(distro_id)); 
    distro_id[strlen(distro_id) - 1] = 0; // remove end quote
    
    return distro_id;
}

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

char *get_uptime(size_t size)
{
    FILE *file = fopen("/proc/uptime", "r");
    if (!file)
    {
        fprintf(stderr, "error: uptime is inaccessible\n");
        return NULL;
    }

    char *uptime;
    unsigned int uptime_seconds = -1; // default value is integer limit, making errors obvious
    uptime = malloc((size+1)*sizeof(*uptime));

    fscanf(file, "%u", &uptime_seconds); // store value of file to an unsigned integer, flooring (rounding) it
    // format uptime string accordingly
    if (uptime_seconds >= 60)
    {
        switch (uptime_seconds%60) // switch seconds
        {
            case 0:
                sprintf(uptime, "%u minutes", uptime_seconds/60);
                break;
            case 1:
                sprintf(uptime, "%u minutes and 1 second", uptime_seconds/60);
                break;
            default:
                sprintf(uptime, "%u minutes and %u seconds", uptime_seconds/60, uptime_seconds%60);
        }
    }
    else
    {
        sprintf(uptime, "%u seconds", uptime_seconds);
    }

    fclose(file);

    return uptime;
}

void info_print(char accent_color_char, bool accent_bold, bool use_order_file, char *order_filename, size_t order_filesize, struct SystemInfo system_info)
{
    char *ansi_accent_color = get_ansi_color_from(accent_color_char, accent_bold);
    char *order;
    order = malloc((order_filesize+1)*sizeof(*order));
    sprintf(order, "%c%c%c%c%c%c", CHAR_ASCII, CHAR_DISTRO_ID, CHAR_PACKAGE_AMOUNT, CHAR_SHELL, CHAR_UPTIME, CHAR_KERNEL_VERSION);
    if (access(order_filename, R_OK) == -1)
    {
        fprintf(stderr, "error: '%s' is unreadable or doesn't exist\n", order_filename);
    }
    else if (use_order_file)
    {
        order = file_read(order_filename, order_filesize);
    }


    for (int i = 0; i <= sizeof(order); ++i)
    {
        switch (order[i]) {
            case CHAR_ASCII:          printf("%s%s%s\n",         ansi_accent_color, system_info.ascii, ANSI_COLOR_RESET); break;
            case CHAR_DISTRO_ID:      printf("%sdistro:%s %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.distro_id); break;
            case CHAR_KERNEL_VERSION: printf("%skernel:%s %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.kernel_version); break;
            case CHAR_PACKAGE_AMOUNT: printf("%spkgs:%s   %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.package_amount); break;
            case CHAR_SHELL:          printf("%sshell:%s  %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.shell); break;
            case CHAR_UPTIME:         printf("%suptime:%s %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.uptime); break;
        }
    }

    free(order);
}
