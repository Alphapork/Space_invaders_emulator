#include <stdio.h>

void dissasemble8080_ROM(unsigned char *codeBuffer, int arraySize) {
  for (int pc = 0; pc < arraySize; pc++) {
    switch ((int)codeBuffer[pc]) {
      case 0x00: printf("%04X   NOP\n",pc); break;
      case 0x01: printf("%04X   LXI  B   #$%02X%02X\n", pc, codeBuffer[pc+1], codeBuffer[pc+2]); pc = pc +3; break;
      case 0x02: printf("%04X   STAX B\n",pc); break;
      case 0x03: printf("%04X   INX  B\n",pc); break;
    }






  }
}


void print_code_in_hex(unsigned char *codeBuffer, int arraySize) {

  int antal = 0;
  for (int i = 0; i < arraySize; i++) {
    antal++;
      printf("%02X  ", codeBuffer[i]);
    if (antal == 16){
      printf("\n");
      antal = 0;
    }
  }


}


int main() {
  FILE *fp;

  // Open ROM and fill array with its contents
  fp = fopen("C://Users/Joel/Documents/Code/Space_invaders_emu/ROMs/invaders.h", "rb");
  fseek(fp, 0, SEEK_END);
  int sizeOfFile = ftell(fp);
  rewind(fp);
  unsigned char buffer[sizeOfFile]; //buffer array which contains binary code
  fread(buffer, sizeOfFile, 1, fp);
  fclose(fp);
  // close file




  print_code_in_hex(buffer, sizeOfFile);
  printf("\n\nSize: %d bytes\n\nPress enter to dissasemble.\n",sizeOfFile);
  getchar();
  dissasemble8080_ROM(buffer, sizeOfFile);

  return 0;
}
