#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "lowfetch.h"

/* modules*/
#include "include/color_definitions.h"
#include "include/package_amount.h"
#include "include/kernel_version.h"
#include "include/lowfetch_base.h"
#include "include/argument_parsing.h"
#include "include/uptime.h"
#include "include/sizes.h"

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
    CHAR_UPTIME = 'u',
    CHAR_XDG_DESKTOP = 'x'
};

struct SystemInfo
{
    char *ascii;
    char *distro_id;
    char *kernel_version;
    char *package_amount;
    char *shell;
    char *uptime;
    char *xdg_desktop;
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
    const char *argdef_bold[] =           {"-b",  "--bold",           "toggle bold colors"};
    const char *argdef_kernel_shorten[] = {"-ks", "--kernel-shorten", "shorten kernel version"};
    // arguments to include in help menu
    // note: new arguments must be appended to end of argdefs_list
    #define ARGDEFS_LIST_SIZE 6
    const char **argdefs_list[ARGDEFS_LIST_SIZE] = {argdef_help, argdef_ascii_file, argdef_order_file, argdef_color, argdef_bold, argdef_kernel_shorten};
    struct Options option = argument_parsing(argc, argv, argdefs_list, ARGDEFS_LIST_SIZE);
    #undef ARGDEFS_LIST_SIZE

    // use arguments
    use_ascii_file = option.use_ascii_file;
    use_order_file = option.use_order_file;
    accent_bold = option.accent_bold;
    shorten_kernel_version = option.shorten_kernel_version;
    if (strcmp("", option.ascii_filename)!=0) { strcpy(ascii_filename, option.ascii_filename); }
    if (strcmp("", option.order_filename)!=0) { strcpy(order_filename, option.order_filename); }
    accent_color_char = option.accent_color_char;

    /* requires freeing */
    char *ascii = get_ascii(use_ascii_file, ascii_filename, ASCII_FILESIZE);
    char *distro_id = get_distro_id(DISTRO_ID_SIZE);
    char *kernel_version = get_kernel_version(shorten_kernel_version, KERNEL_VERSION_SIZE);
    char *package_amount = get_package_amount(PACKAGE_AMOUNT_SIZE);
    char *uptime = get_uptime(UPTIME_SIZE);

    struct SystemInfo system_info = 
    {
        .ascii = ascii,
        .distro_id = distro_id,
        .kernel_version = kernel_version,
        .package_amount = package_amount,
        .shell = get_shell(),
        .uptime = uptime,
        .xdg_desktop = get_xdg_desktop()
    };

    info_print(accent_color_char, accent_bold, use_order_file, order_filename, ORDER_FILESIZE, system_info);

    free(ascii);
    free(distro_id);
    free(kernel_version);
    free(package_amount);
    free(uptime);
    return 0;
}

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

char *get_xdg_desktop() {
    // this prefers $XDG_CURRENT_DESKTOP over $XDG_SESSION_DESKTOP
    if (!getenv("XDG_SESSION_DESKTOP") && !getenv("XDG_CURRENT_DESKTOP")) {
        return "unknown";
    }
    
    if (getenv("XDG_CURRENT_DESKTOP")) {
        return getenv("XDG_CURRENT_DESKTOP");
    }
    
    return getenv("XDG_SESSION_DESKTOP");
}

void info_print(char accent_color_char, bool accent_bold, bool use_order_file, char *order_filename, size_t order_filesize, struct SystemInfo system_info)
{
    char *ansi_accent_color = get_ansi_color_from(accent_color_char, accent_bold);
    char *order;
    order = malloc((order_filesize+1)*sizeof(*order));
    sprintf(order, "%c%c%c%c%c%c%c", CHAR_ASCII, CHAR_DISTRO_ID, CHAR_XDG_DESKTOP, CHAR_PACKAGE_AMOUNT, CHAR_SHELL, CHAR_UPTIME, CHAR_KERNEL_VERSION);
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
            case CHAR_XDG_DESKTOP:    printf("%sxdg:%s    %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.xdg_desktop); break;
        }
    }

    free(order);
}
