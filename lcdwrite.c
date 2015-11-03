// I2C test program for a AQM0802A LCD

#include <stdlib.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>

#define ASCIITABLESIZE 96
#define DISPLAYSIZE 8
#define DISPLAYLINES 2

char * charlookup(char syschar) {
  char * lcdcode;
  char chartable[ASCIITABLESIZE][2];
  int i;

   chartable[0][0] = ' ';   chartable[0][1] = 0x20;
   chartable[1][0] = '!';   chartable[1][1] = 0x21;
   chartable[2][0] = '"';   chartable[2][1] = 0x22;
   chartable[3][0] = '#';   chartable[3][1] = 0x23;
   chartable[4][0] = '$';   chartable[4][1] = 0x24;
   chartable[5][0] = '%';   chartable[5][1] = 0x25;
   chartable[6][0] = '&';   chartable[6][1] = 0x26;
   chartable[7][0] = '\'';  chartable[7][1] = 0x27;
   chartable[8][0] = '(';   chartable[8][1] = 0x28;
   chartable[9][0] = ')';   chartable[9][1] = 0x29;
  chartable[10][0] = '*';  chartable[10][1] = 0x2A;
  chartable[11][0] = '+';  chartable[11][1] = 0x2B;
  chartable[12][0] = ',';  chartable[12][1] = 0x2C;
  chartable[13][0] = '-';  chartable[13][1] = 0x2D;
  chartable[14][0] = '.';  chartable[14][1] = 0x2E;
  chartable[15][0] = '/';  chartable[15][1] = 0x2F;
  chartable[16][0] = '0';  chartable[16][1] = 0x30;
  chartable[17][0] = '1';  chartable[17][1] = 0x31;
  chartable[18][0] = '2';  chartable[18][1] = 0x32;
  chartable[19][0] = '3';  chartable[19][1] = 0x33;
  chartable[20][0] = '4';  chartable[20][1] = 0x34;
  chartable[21][0] = '5';  chartable[21][1] = 0x35;
  chartable[22][0] = '6';  chartable[22][1] = 0x36;
  chartable[23][0] = '7';  chartable[23][1] = 0x37;
  chartable[24][0] = '8';  chartable[24][1] = 0x38;
  chartable[25][0] = '9';  chartable[25][1] = 0x39;
  chartable[26][0] = ':';  chartable[26][1] = 0x3A;
  chartable[27][0] = ';';  chartable[27][1] = 0x3B;
  chartable[28][0] = '<';  chartable[28][1] = 0x3C;
  chartable[29][0] = '=';  chartable[29][1] = 0x3D;
  chartable[30][0] = '>';  chartable[30][1] = 0x3E;
  chartable[31][0] = '?';  chartable[31][1] = 0x3F;
  chartable[32][0] = '@';  chartable[32][1] = 0x40;
  chartable[33][0] = 'A';  chartable[33][1] = 0x41;
  chartable[34][0] = 'B';  chartable[34][1] = 0x42;
  chartable[35][0] = 'C';  chartable[35][1] = 0x43;
  chartable[36][0] = 'D';  chartable[36][1] = 0x44;
  chartable[37][0] = 'E';  chartable[37][1] = 0x45;
  chartable[38][0] = 'F';  chartable[38][1] = 0x46;
  chartable[39][0] = 'G';  chartable[39][1] = 0x47;
  chartable[40][0] = 'H';  chartable[40][1] = 0x48;
  chartable[41][0] = 'I';  chartable[41][1] = 0x49;
  chartable[42][0] = 'J';  chartable[42][1] = 0x4A;
  chartable[43][0] = 'K';  chartable[43][1] = 0x4B;
  chartable[44][0] = 'L';  chartable[44][1] = 0x4C;
  chartable[45][0] = 'M';  chartable[45][1] = 0x4D;
  chartable[46][0] = 'N';  chartable[46][1] = 0x4E;
  chartable[47][0] = 'O';  chartable[47][1] = 0x4F;
  chartable[48][0] = 'P';  chartable[48][1] = 0x50;
  chartable[49][0] = 'Q';  chartable[49][1] = 0x51;
  chartable[50][0] = 'R';  chartable[50][1] = 0x52;
  chartable[51][0] = 'S';  chartable[51][1] = 0x53;
  chartable[52][0] = 'T';  chartable[52][1] = 0x54;
  chartable[53][0] = 'U';  chartable[53][1] = 0x55;
  chartable[54][0] = 'V';  chartable[54][1] = 0x56;
  chartable[55][0] = 'W';  chartable[55][1] = 0x57;
  chartable[56][0] = 'X';  chartable[56][1] = 0x58;
  chartable[57][0] = 'Y';  chartable[57][1] = 0x59;
  chartable[58][0] = 'Z';  chartable[58][1] = 0x5A;
  chartable[59][0] = '[';  chartable[59][1] = 0x5B;
  chartable[60][0] = ' ';  chartable[60][1] = 0x5C; // The Yen currency symbol
  chartable[61][0] = ']';  chartable[61][1] = 0x5D;
  chartable[62][0] = '^';  chartable[62][1] = 0x5E;
  chartable[63][0] = '_';  chartable[63][1] = 0x5F;
  chartable[64][0] = '\\'; chartable[64][1] = 0x60;
  chartable[65][0] = 'a';  chartable[65][1] = 0x61;
  chartable[66][0] = 'b';  chartable[66][1] = 0x62;
  chartable[67][0] = 'c';  chartable[67][1] = 0x63;
  chartable[68][0] = 'd';  chartable[68][1] = 0x64;
  chartable[69][0] = 'e';  chartable[69][1] = 0x65;
  chartable[70][0] = 'f';  chartable[70][1] = 0x66;
  chartable[71][0] = 'g';  chartable[71][1] = 0x67;
  chartable[72][0] = 'h';  chartable[72][1] = 0x68;
  chartable[73][0] = 'i';  chartable[73][1] = 0x69;
  chartable[74][0] = 'j';  chartable[74][1] = 0x6A;
  chartable[75][0] = 'k';  chartable[75][1] = 0x6B;
  chartable[76][0] = 'l';  chartable[76][1] = 0x6C;
  chartable[77][0] = 'm';  chartable[77][1] = 0x6D;
  chartable[78][0] = 'n';  chartable[78][1] = 0x6E;
  chartable[79][0] = 'o';  chartable[79][1] = 0x6F;
  chartable[80][0] = 'p';  chartable[80][1] = 0x70;
  chartable[81][0] = 'q';  chartable[81][1] = 0x71;
  chartable[82][0] = 'r';  chartable[82][1] = 0x72;
  chartable[83][0] = 's';  chartable[83][1] = 0x73;
  chartable[84][0] = 't';  chartable[84][1] = 0x74;
  chartable[85][0] = 'u';  chartable[85][1] = 0x75;
  chartable[86][0] = 'v';  chartable[86][1] = 0x76;
  chartable[87][0] = 'w';  chartable[87][1] = 0x77;
  chartable[88][0] = 'x';  chartable[88][1] = 0x78;
  chartable[89][0] = 'y';  chartable[89][1] = 0x79;
  chartable[90][0] = 'z';  chartable[90][1] = 0x7A;
  chartable[91][0] = '{';  chartable[91][1] = 0x7B;
  chartable[92][0] = '|';  chartable[92][1] = 0x7C;
  chartable[93][0] = '}';  chartable[93][1] = 0x7D;
  chartable[94][0] = ' ';  chartable[94][1] = 0x7E; // arrow pointing right
  chartable[95][0] = ' ';  chartable[95][1] = 0x7F; // arrow pointing left

  lcdcode = NULL;
  for(i=0;i<ASCIITABLESIZE;i++) {
    if(syschar==chartable[i][0]) break;
  }
  lcdcode = &chartable[i][1];
  return lcdcode;
}

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

void lcdputchar(char displaychar) {
  char * lcdchar;
  char bytes[2];
  lcdchar = charlookup(displaychar);
  if(lcdchar==NULL) return;
  bytes[0] = 0x40;
  bytes[1] = *lcdchar;
  write(i2cFile, bytes, sizeof(bytes));
}

void lcdwritestr(int line, char s[]) {
  int i;
  char bytes[2];
  bytes[0] = 0x00;
  if (line == 1)  bytes[1] = 0x80;          // Set display address for line 1
  if (line == 2)  bytes[1] = 0xc0;          // Set display address for line 2
  write(i2cFile, bytes, sizeof(bytes));
  for(i=0;i<DISPLAYSIZE && s[i];i++)         // Send only max chars to display
    lcdputchar(s[i]);                        // when the given string is longer
}

int main(int argc, char** argv) {
  char lcdstr[9];
  if (argc != 3) return -1;
  // open Linux I2C device
  i2cOpen();

  // set address of the AQM0802A
  i2cSetAddress(0x3e);

  lcdstr[0] = '\0';
  // handle strings that are too long
  strncpy(lcdstr, argv[2], 8);

  // handle strings that are too short
  if (strlen(lcdstr) < DISPLAYSIZE) {
     int i;
     for (i=strlen(lcdstr); i<DISPLAYSIZE; i++) {
       lcdstr[i] = ' ';
     }
     lcdstr[8] = '\0';
  }

  lcdwritestr( atoi(argv[1]), lcdstr);

  // close Linux I2C device
  i2cClose();

  return 0;
}
