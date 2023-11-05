#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* modules*/
#include "include/argument_parsing/argument_parsing.h"
#include "include/ascii/ascii.h"
#include "include/distro/distro.h"
#include "include/kernel/kernel.h"
#include "include/output/output.h"
#include "include/package_amount/package_amount.h"
#include "include/shell/shell.h"
#include "include/uptime/uptime.h"
#include "include/xdg_desktop/xdg_desktop.h"

int main(int argc, char **argv)
{
    Options options = argument_parsing(argc, argv);

    /* requires freeing */
    char *ascii = get_ascii(options.use_ascii_file, options.ascii_file);
    char *distro_id = get_distro_id();
    char *kernel_version = get_kernel_version(options.kernel_shorten);
    char *package_amount = get_package_amount();
    char *uptime = get_uptime();

    SystemInfo system_info =
    {
        .ascii = ascii,
        .distro_id = distro_id,
        .kernel_version = kernel_version,
        .package_amount = package_amount,
        .shell = get_shell(),
        .uptime = uptime,
        .xdg_desktop = get_xdg_desktop()
    };

    info_print(options.color_char, options.bold, options.use_order_file, options.order_file, system_info);

    free(ascii);
    free(distro_id);
    free(kernel_version);
    free(package_amount);
    free(uptime);
    return 0;
}
