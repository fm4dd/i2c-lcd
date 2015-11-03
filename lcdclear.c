/* -------------------------------------- *
 * I2C control program for a AQM0802A LCD *
 * This is the LCD display clear command. *
 *                                        *
 * 2015-12-21 support@frank4dd.com        *
 * -------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

// I2C Linux device handle
int i2cFile;

// open the Linux device
void i2cOpen() {
   i2cFile = open("/dev/i2c-1", O_RDWR);
   if (i2cFile < 0) {
      perror("i2cOpen");
      exit(1);
   }
}

// close the Linux device
void i2cClose() { close(i2cFile); }

// set the I2C slave address for all subsequent I2C device transfers
void i2cSetAddress(int address) {
   if (ioctl(i2cFile, I2C_SLAVE, address) < 0) {
      perror("i2cSetAddress");
      exit(1);
   }
}

void lcd_clear() {
   char bytes[] = { 0x00, 0x08 };
   write(i2cFile, bytes, sizeof(bytes));
}

int main(int argc, char** argv) {
   if (argc != 3) return -1;
   // open Linux I2C device
   i2cOpen();

   // set address of the AQM0802A
   i2cSetAddress(0x3e);

   // initialize lcd
   lcd_clear();
   sleep (0.3);

   // close Linux I2C device
   i2cClose();

   return 0;
}
