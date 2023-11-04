#include <stdio.h>
#include <unistd.h>
#include "../definitions/colors.h"
#include "../general/file_read.h"
#include "output.h"

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
