#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../definitions/colors.h"
#include "../general/file_read.h"
#include "../definitions/sizes.h"
#include "output.h"

void info_print(char color_char, bool bold, bool use_order_file, char *order_filename, SystemInfo system_info)
{
    char *ansi_accent_color = get_ansi_color_from(color_char, bold);
    char *order = malloc((ORDER_FILESIZE+1)*sizeof(*order));

    sprintf(order, "%c%c%c%c%c%c%c", CHAR_ASCII, CHAR_DISTRO_ID, CHAR_XDG_DESKTOP, CHAR_PACKAGE_AMOUNT, CHAR_SHELL, CHAR_UPTIME, CHAR_KERNEL_VERSION);
    if (access(order_filename, R_OK) == -1)
    {
        fprintf(stderr, "error: '%s' (order) is unreadable or doesn't exist\n", order_filename);
    }
    else if (use_order_file)
    {
        order = file_read(order_filename, ORDER_FILESIZE);
    }

    for (int i = 0; i <= sizeof(order); ++i)
    {
        switch (order[i]) {
            case CHAR_ASCII:          printf("%s%s%s\n",      ansi_accent_color, system_info.ascii, ANSI_COLOR_RESET);           break;
            case CHAR_DISTRO_ID:      printf("%sdis:%s %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.distro_id);      break;
            case CHAR_KERNEL_VERSION: printf("%sker:%s %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.kernel_version); break;
            case CHAR_PACKAGE_AMOUNT: printf("%spkg:%s %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.package_amount); break;
            case CHAR_SHELL:          printf("%sshl:%s %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.shell);          break;
            case CHAR_UPTIME:         printf("%supt:%s %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.uptime);         break;
            case CHAR_XDG_DESKTOP:    printf("%sxdg:%s %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.xdg_desktop);    break;
        }
    }

    free(order);
}
