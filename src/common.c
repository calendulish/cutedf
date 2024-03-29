/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2004  Roman Bogorodskiy <bogorodskiy@inbox.ru>
 *                     Pierre Chifflier <chifflier@cpe.fr>
 *               2016  Lara Maia <dev@lara.click>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "colors.h"

double blocksize;
char bs_suff;
short human_readable;

short show_pseudofs;

int header_showed = 0;

char *bar(unsigned short n) {
    int i;
    char *gauge = (char *) malloc(11);
    char *ret = (char *) malloc(64);

    *gauge = '\0'; /* avoid apparition of artefacts: strcat on non-initialized memory ... */

    if (n > 10)
        return NULL;

    for (i = 1; i <= 10; i++) {
        if (i <= n)
            gauge = strcat(gauge, ">");
        else
            gauge = strcat(gauge, " ");
    }

    sprintf(ret, "%s[%s%s%s]%s", bracket_color, gauge_color,
            gauge, bracket_color, NORMAL);


    return ret;
}

void header() {
    printf("%s", header_color);

    if (header_showed == 0) {
        printf("%-10s %12s %7s %7s %7s %-18s %s\n",
               "Filesystem",
               "Type",
               "Free",
               "Used",
               "Total",
               "Mnt pnt",
               "Capacity"
        );
        header_showed = 1;
    }

    printf("%s", NORMAL);
}

short is_pseudofs(const char *fs_type) {
    /* XXX:
       - this list is not complete yet
       - I'm not sure how it will work on Linux
    */

    if ((strcmp(fs_type, "devfs") == 0) ||
        (strcmp(fs_type, "devpts") == 0) ||
        (strcmp(fs_type, "proc") == 0) ||
        (strcmp(fs_type, "sysfs") == 0) ||
        (strcmp(fs_type, "usbfs") == 0)) {
        return 1;
    }

    return 0;
}

char *numeric_value(const double bytes) {
    char *buf;

    buf = (char *) malloc(256);

    if (human_readable) {
        if ((bytes / 1073741824) >= 1) {
            (void) snprintf(buf, 255, "%6.0fG", (bytes / 1073741824));
        } else if ((bytes / 1048576) >= 1) {
            (void) snprintf(buf, 255, "%6.0fM", (bytes / 1048576));
        } else if ((bytes / 1024) >= 1) {
            (void) snprintf(buf, 255, "%6.0fK", (bytes / 1024));
        } else {
            (void) snprintf(buf, 255, "%7.0f", bytes);
        }
    } else
        (void) snprintf(buf, 255, "%7.0f", bytes);

    return buf;
}
