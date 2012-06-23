// $ gcc -g -o gpiotest gpiotest.c -Lbuild -lbone -Isrc/
// # LD_LIBRARY_PATH="build/" ./gpiotest
#include <stdio.h>
#include <unistd.h>
#include "libbone.h"

int main(void)
{
    int i, j, k;

    bone_gpio_export(P8+3);
    i = bone_gpio_get_dir(P8+3);
    bone_gpio_set_dir(P8+3, 1); /* Output */
    bone_gpio_set_value(P8+3, 0);
    for (j = 0; j < 10; j++) {
        for (k = 0; k < 4; k++) {
            bone_gpio_set_value(P8+3, 1);
            usleep(6666);
            bone_gpio_set_value(P8+3, 0);
            usleep(160000);
        }
        usleep(333333);
    }

    bone_gpio_unexport(P8+3);
    return 0;
}
