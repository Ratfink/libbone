// $ gcc -g -o printoled printoled.c -Lbuild -lbone -Isrc/
// # LD_LIBRARY_PATH="build/" ./printoled
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include "libbone.h"
#include "ssd1306.h"

#define DEBOUNCE_DELAY 150000
#define SCROLLBACK 256


int get_lead(int fd)
{
    int value;
    lseek(fd, 0, 0);

    char buffer[2];
    int size = read(fd, buffer, sizeof(buffer));
    if (size != -1) {
        buffer[size] = '\0';
        value = atoi(buffer);
    }
    else {
        value = -1;
    }

    return value;
}


int main(int argc, char *argv[])
{
    int i;
    int numread;
    char *ss[SCROLLBACK];
    int pos = SCROLLBACK - 8;
    char *st;
    bone_ssd1306_t *display = bone_ssd1306_init(P8+3, I2C, 1, 0x3c, 128, 64);
    struct pollfd pfd[6];
    bool eof_seen = 0;
    int charisspace;

    bone_gpio_export(P9+14);
    bone_gpio_export(P9+15);
    bone_gpio_export(P9+16);
    bone_gpio_export(P9+23);
    bone_gpio_export(P9+25);
    bone_gpio_set_dir(P9+14, 0);
    bone_gpio_set_dir(P9+15, 0);
    bone_gpio_set_dir(P9+16, 0);
    bone_gpio_set_dir(P9+23, 0);
    bone_gpio_set_dir(P9+25, 0);
    bone_gpio_set_edge(P9+14, (enum bone_gpio_edge) RISING);
    bone_gpio_set_edge(P9+15, (enum bone_gpio_edge) RISING);
    bone_gpio_set_edge(P9+16, (enum bone_gpio_edge) RISING);
    bone_gpio_set_edge(P9+23, (enum bone_gpio_edge) RISING);
    bone_gpio_set_edge(P9+25, (enum bone_gpio_edge) RISING);

    pfd[0].fd = 0;
    pfd[0].events = POLLIN;
    pfd[0].revents = 0;
    pfd[1].fd = bone_gpio_open_value(P9+14);
    pfd[2].fd = bone_gpio_open_value(P9+15);
    pfd[3].fd = bone_gpio_open_value(P9+16);
    pfd[4].fd = bone_gpio_open_value(P9+23);
    pfd[5].fd = bone_gpio_open_value(P9+25);
    for (i = 1; i <= 5; i++) {
        if (pfd[i].fd < 0) {
            bone_ssd1306_free(display);
            return -1;
        }
    }
    for (i = 1; i <= 5; i++) {
        pfd[i].events = POLLPRI | POLLERR;
        pfd[i].revents = 0;
    }

    for (i = 0; i < SCROLLBACK; i++) {
        ss[i] = calloc(22, 1);
        if (ss[i] == NULL)
            return 1;
    }
    bone_ssd1306_setup(display);

    bone_ssd1306_clear(display, 0);
    bone_ssd1306_line(display, 126, 0, 126, 63, 1);
    bone_ssd1306_line(display, 127, 0, 127, 63, 1);
    bone_ssd1306_draw(display);
    while (!eof_seen) {
        poll(pfd, 6, -1);
        bone_ssd1306_clear(display, 0);
        if (pfd[0].revents != 0) {
            do {
                numread = read(0, ss[0], 21);
                if (numread <= 0)
                    eof_seen = true;
                ss[0][numread] = '\0';
                for (i = 0; i < 21; i++) {
                    if (ss[0][i] == '\0')
                        break;
                    charisspace = isspace(ss[0][i]);
                    if (!charisspace)
                        break;
                }
                if (charisspace)
                    break;
                st = ss[0];
                for (i = 1; i < SCROLLBACK; i++) {
                    ss[i-1] = ss[i];
                }
                ss[SCROLLBACK-1] = st;
                poll(pfd, 1, 0);
            } while (pfd[0].revents != 0 && !eof_seen);
        }
        if (pfd[1].revents & POLLPRI) {
            get_lead(pfd[1].fd);
            usleep(DEBOUNCE_DELAY);
            get_lead(pfd[1].fd);
            if (pos > 0)
                pos -= 1;
        }
        if (pfd[2].revents & POLLPRI) {
            get_lead(pfd[2].fd);
            usleep(DEBOUNCE_DELAY);
            get_lead(pfd[2].fd);
            if (pos < SCROLLBACK - 8)
                pos += 1;
        }
        for (i = 0; i < 8; i++) {
            bone_ssd1306_str(display, 0, 56-8*i, 1, ss[i+pos]);
        }
        bone_ssd1306_line(display, 126, 64-64*pos/SCROLLBACK-64*8/SCROLLBACK,
                126, 64-64*pos/SCROLLBACK, 1);
        bone_ssd1306_line(display, 127, 64-64*pos/SCROLLBACK-64*8/SCROLLBACK,
                127, 64-64*pos/SCROLLBACK, 1);
        bone_ssd1306_draw(display);
    }

    bone_ssd1306_free(display);
    for (i = 1; i <= 5; i++)
        close(pfd[i].fd);
    bone_gpio_unexport(P9+14);
    bone_gpio_unexport(P9+15);
    bone_gpio_unexport(P9+16);
    bone_gpio_unexport(P9+23);
    bone_gpio_unexport(P9+25);
    for (i = 0; i < SCROLLBACK; i++)
        free(ss[i]);

    return 0;
}
