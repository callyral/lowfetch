#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* modules*/
#include "include/general/file_read.h"
#include "include/definitions/colors.h"
#include "include/definitions/sizes.h"
#include "include/package_amount/package_amount.h"
#include "include/kernel/kernel.h"
#include "include/argument_parsing/argument_parsing.h"
#include "include/uptime/uptime.h"
#include "include/distro/distro.h"
#include "include/shell/shell.h"
#include "include/session/session.h"
#include "include/output/output.h"
#include "include/ascii/ascii.h"

int main(int argc, char **argv)
{
    bool accent_bold = false;
    char accent_color_char = CHAR_WHITE;

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
    char *kernel_version = get_kernel_version(shorten_kernel_version, KERNEL_SIZE);
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
        .xdg_desktop = get_desktop_session()
    };

    info_print(accent_color_char, accent_bold, use_order_file, order_filename, ORDER_FILESIZE, system_info);

    free(ascii);
    free(distro_id);
    free(kernel_version);
    free(package_amount);
    free(uptime);
    return 0;
}
