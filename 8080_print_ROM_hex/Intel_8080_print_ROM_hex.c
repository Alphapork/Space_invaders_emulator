#include <stdio.h>
#include <stdint.h>


void print_code_in_hex(uint8_t *codeBuffer, int arraySize) {

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


int main(int argc, char **argv) {



  // Open ROM and fill array with its contents
  FILE *fp;
  fp = fopen(argv[1], "rb");
  fseek(fp, 0, SEEK_END);
  int size_of_ROM = ftell(fp);
  rewind(fp);
  uint8_t ROM_code_buffer[size_of_ROM]; //buffer array which contains binary code
  fread(ROM_code_buffer, size_of_ROM, 1, fp);
  fclose(fp);   // close file



  print_code_in_hex(ROM_code_buffer, size_of_ROM);

  printf("\n\nSize: %d bytes\n\n",size_of_ROM);


  return 0;
}
