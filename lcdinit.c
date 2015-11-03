/* -------------------------------------- *
 * I2C control program for a AQM0802A LCD *
 * This is the LCD display init command.  *
 *                                        *
 * 2015-12-21 support@frank4dd.com        *
 * -------------------------------------- */
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>

// I2C Linux device handle
int i2cFile;

// open the Linux device
void i2cOpen() {
   i2cFile = open("/dev/i2c-2", O_RDWR);
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

void lcd_config() {
   char initseq[] = { 0x00, 0x38, 0x39, 0x14, 0x70, 0x56, 0x6c };
   char dispon[] = { 0x00, 0x38, 0x0c, 0x01 };
   char increm[] = { 0x00, 0x38, 0x06 };

   write(i2cFile, initseq, sizeof(initseq));
//   sleep (0.3);
   write(i2cFile, dispon, sizeof(dispon));
//   sleep (0.3);
   write(i2cFile, increm, sizeof(increm));
//   sleep (0.3);
}

void lcd_clear() {
   char bytes[] = { 0x00, 0x01 };
   write(i2cFile, bytes, sizeof(bytes));
}

void lcd_off() {
   char bytes[] = { 0x00, 0x08 };
   write(i2cFile, bytes, sizeof(bytes));
}

int main(int argc, char** argv) {
   // open Linux I2C device
   i2cOpen();

   // set address of the AQM0802A
   i2cSetAddress(0x3e);

   // initialize lcd
   lcd_config();

   // close Linux I2C device
   i2cClose();

   return 0;
}
