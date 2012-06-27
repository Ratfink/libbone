// $ gcc -g -o oled oled.c -Lbuild -lbone -Isrc/
// # LD_LIBRARY_PATH="build/" ./oled
#include <stdio.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include "libbone.h"
#include "oled.h"


int oled_command(int fd, unsigned int cmd)
{
    cmd &= ~0x100;
    struct spi_ioc_transfer trans = {
        .tx_buf = (unsigned long)&cmd,
        .rx_buf = (unsigned long)NULL,
        .len = 2,
        .delay_usecs = 0,
        .speed_hz = 10000000,
        .bits_per_word = 9
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &trans) == -1) {
        fprintf(stderr, "FRIIG\n");
        return -1;
    }
}


int oled_data(int fd, unsigned int dat)
{
    dat |= 0x100;
    struct spi_ioc_transfer trans = {
        .tx_buf = (unsigned long)&dat,
        .rx_buf = (unsigned long)NULL,
        .len = 2,
        .delay_usecs = 0,
        .speed_hz = 10000000,
        .bits_per_word = 9
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &trans) == -1) {
        fprintf(stderr, "FRIIIG\n");
        return -1;
    }
}


int main(void)
{
    int i;
    int fd;

    bone_gpio_export(P8+3);
    bone_gpio_set_dir(P8+3, 1);
    bone_gpio_set_value(P8+3, 0);
    usleep(10000);
    bone_gpio_set_value(P8+3, 1);

    if ((fd = open("/dev/spidev2.0", O_RDWR)) == -1) {
        fprintf(stderr, "FRIG\n");
        return 1;
    }

    oled_command(fd, 0xae);//--turn off oled panel
    oled_command(fd, 0x40);//--set start line address
    oled_command(fd, 0x20);
    oled_command(fd, 0x01);
    oled_command(fd, 0x81);//--set contrast control register
    oled_command(fd, 0xcf);
    oled_command(fd, 0xa1);//--set segment re-map 95 to 0
    oled_command(fd, 0xa7);//--set inverse display
    oled_command(fd, 0xa8);//--set multiplex ratio(1 to 64)
    oled_command(fd, 0x3f);//--1/64 duty
    oled_command(fd, 0xd3);//-set display offset
    oled_command(fd, 0x00);//-not offset
    oled_command(fd, 0xd5);//--set display clock divide ratio/oscillator frequency
    oled_command(fd, 0xf0);//--set divide ratio
    oled_command(fd, 0xd9);//--set pre-charge period
    oled_command(fd, 0xf1);
    oled_command(fd, 0xda);//--set com pins hardware configuration
    oled_command(fd, 0x12);
    oled_command(fd, 0xdb);//--set vcomh
    oled_command(fd, 0x40);
    oled_command(fd, 0x8d);//--set Charge Pump enable/disable
    oled_command(fd, 0x14);//--set(0x10) disable
    oled_command(fd, 0xaf);//--turn on oled panel

    /* program goes here */
    for (i = 0; i < (128*64/8); i++) {
        oled_data(fd, 0x00);
    }
    for (i = 0; i < (nepeta_leijon_bw_width*nepeta_leijon_bw_height/8); i++) {
//        oled_data(fd, 0xfe);
        oled_data(fd, nepeta_leijon_bw_bits[i]);
    }
    sleep(10);

    oled_command(fd, 0xae);
    if (close(fd) == -1) {
        fprintf(stderr, "FRIIIIG\n");
        return 1;
    }
    bone_gpio_unexport(P8+3);

    return 0;
}
