// $ gcc -g -o printoled printoled.c -Lbuild -lbone -Isrc/
// # LD_LIBRARY_PATH="build/" ./printoled
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
    int fd[3];
    int i;
    int numread;
    char *ss[SCROLLBACK];
    int pos = SCROLLBACK - 8;
    char *st;
    bone_ssd1306_t *display = bone_ssd1306_init(P8+3, 2, 0, 128, 64);
    struct pollfd pfd[3];

    bone_gpio_export(P9+12);
    bone_gpio_export(P9+13);
    bone_gpio_set_dir(P9+12, 0);
    bone_gpio_set_dir(P9+13, 0);
    bone_gpio_set_edge(P9+12, (enum bone_gpio_edge) FALLING);
    bone_gpio_set_edge(P9+13, (enum bone_gpio_edge) FALLING);
    fd[0] = 0;
    fd[1] = bone_gpio_open_value(P9+12);
    if (fd[1] < 0)
        return -1;
    fd[2] = bone_gpio_open_value(P9+13);
    if (fd[2] < 0)
        return -1;
    pfd[0].fd = fd[0];
    pfd[0].events = POLLIN;
    pfd[0].revents = 0;
    pfd[1].fd = fd[1];
    pfd[1].events = POLLPRI | POLLERR;
    pfd[1].revents = 0;
    pfd[2].fd = fd[2];
    pfd[2].events = POLLPRI | POLLERR;
    pfd[2].revents = 0;

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
/*    while (fgets(ss[0], 22, stdin) > 0) { */
    for (;;) {
        poll(pfd, 3, -1);
        bone_ssd1306_clear(display, 0);
        if (pfd[0].revents != 0) {
            numread = read(0, ss[0], 21);
            if (numread <= 0)
                break;
            ss[0][numread] = '\0';
            st = ss[0];
            for (i = 1; i < SCROLLBACK; i++) {
                ss[i-1] = ss[i];
            }
            ss[SCROLLBACK-1] = st;
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
        bone_ssd1306_line(display, 126, (64-(64*pos/SCROLLBACK))-64*8/SCROLLBACK, 126, 64-(64*pos/SCROLLBACK), 1);
        bone_ssd1306_line(display, 127, (64-(64*pos/SCROLLBACK))-64*8/SCROLLBACK, 127, 64-(64*pos/SCROLLBACK), 1);
        bone_ssd1306_draw(display);
    }

    bone_ssd1306_free(display);
    close(fd[1]);
    close(fd[2]);
    bone_gpio_unexport(P9+12);
    bone_gpio_unexport(P9+13);
/*    for (i = 0; i < SCROLLBACK; i++)
        free(ss[i]);*/

    return 0;
}
