/*
 * This code is based on sunfunder sample code in following repository.
 *  https://github.com/sunfounder/davinci-kit-for-raspberry-pi/blob/master/c/1.1.7/1.1.7_Lcd1602.c
 */
/*
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

/**
 * @file i2c lcd1602 controll
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>

int BLEN = 1;

static const struct i2c_dt_spec lcd1602 = 
				I2C_DT_SPEC_GET(DT_NODELABEL(i2c_lcd1602));

void write_word(uint8_t data){
    uint8_t temp = data;
    if ( BLEN == 1 )
        temp |= 0x08;
    else
        temp &= 0xF7;
    i2c_write_dt(&lcd1602, &temp, 1);
}

void send_command(uint8_t comm){
    uint8_t buf;
    // Send bit7-4 firstly
    buf = comm & 0xF0;
    buf |= 0x04;                    // RS = 0, RW = 0, EN = 1
    write_word(buf);
    k_msleep(2);
    buf &= 0xFB;                    // Make EN = 0
    write_word(buf);

    // Send bit3-0 secondly
    buf = (comm & 0x0F) << 4;
    buf |= 0x04;                    // RS = 0, RW = 0, EN = 1
    write_word(buf);
    k_msleep(2);
    buf &= 0xFB;                    // Make EN = 0
    write_word(buf);
}

void send_data(uint8_t data){
    uint8_t buf;
    // Send bit7-4 firstly
    buf = data & 0xF0;
    buf |= 0x05;                    // RS = 1, RW = 0, EN = 1
    write_word(buf);
    k_msleep(2);
    buf &= 0xFB;                    // Make EN = 0
    write_word(buf);

    // Send bit3-0 secondly
    buf = (data & 0x0F) << 4;
    buf |= 0x05;                    // RS = 1, RW = 0, EN = 1
    write_word(buf);
    k_msleep(2);
    buf &= 0xFB;                    // Make EN = 0
    write_word(buf);
}

void init(){
    send_command(0x33);     // Must initialize to 8-line mode at first
    k_msleep(5);
    send_command(0x32);     // Then initialize to 4-line mode
    k_msleep(5);
    send_command(0x28);     // 2 Lines & 5*7 dots
    k_msleep(5);
    send_command(0x0C);     // Enable display without cursor
    k_msleep(5);
    send_command(0x01);     // Clear Screen
    i2c_write_dt(&lcd1602, 0x8, 1);
}

void clear(){
    send_command(0x01);     //clear Screen
}

void write(uint8_t x, uint8_t y, char data[]){
    uint8_t addr, i;
    uint8_t tmp;
    if (x < 0)  x = 0;
    if (x > 15) x = 15;
    if (y < 0)  y = 0;
    if (y > 1)  y = 1;

    // Move cursor
    addr = 0x80 + 0x40 * y + x;
    send_command(addr);

    tmp = strlen(data);
    for (i = 0; i < tmp; i++){
        send_data(data[i]);
    }
}

int main(void)
{
	printk("start!\n");
	if (!i2c_is_ready_dt(&lcd1602)) {
		return 0;
	}
	printk("device is ready!\n");

	init();
	printk("device init!\n");

	while (1) {
		printk("Zephyr OS\nI2C LCD1602\n\n");
		write(0, 0, "Zephyr OS");
    	write(0, 1, "I2C LCD1602");
		k_msleep(5000);
		clear();
		printk("Smaple code\nis running!\n\n");
		write(0, 0, "Smaple code");
    	write(1, 1, "is running!");
		k_msleep(5000);
		clear();
	}
	return 0;
}
