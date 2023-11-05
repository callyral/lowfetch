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
    Options options = argument_parsing(argc, argv);

    /* requires freeing */
    char *ascii = get_ascii(options.use_ascii_file, options.ascii_file, ASCII_FILESIZE);
    char *distro_id = get_distro_id(DISTRO_ID_SIZE);
    char *kernel_version = get_kernel_version(options.kernel_shorten, KERNEL_SIZE);
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

    info_print(options.color_char, options.bold, options.use_order_file, options.order_file, ORDER_FILESIZE, system_info);

    free(ascii);
    free(distro_id);
    free(kernel_version);
    free(package_amount);
    free(uptime);
    return 0;
}
