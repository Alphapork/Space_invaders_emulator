#include <stdio.h>

void dissasemble8080_ROM(unsigned char *codeBuffer, int arraySize) {
  for (int pc = 0; pc < arraySize; pc++) {
    //Switch for checking op codes
    switch (codeBuffer[pc]) {
      case 0x00: printf("%04X   NOP\n",pc); break;
      case 0x01: printf("%04X   LXI  B   #$%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0x02: printf("%04X   STAX B\n",pc); break;
      case 0x03: printf("%04X   INX  B\n",pc); break;
      case 0x04: printf("%04X   INR  B\n",pc); break;
      case 0x05: printf("%04X   DCR  B\n",pc); break;
      case 0x06: printf("%04X   MVI  B   #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0x07: printf("%04X   RLC\n",pc); break;
      case 0x08: printf("%04X   NOP*\n",pc); break;
      case 0x09: printf("%04X   DAD  B\n",pc); break;
      case 0x0A: printf("%04X   LDAX B\n",pc); break;
      case 0x0B: printf("%04X   DCX  B\n",pc); break;
      case 0x0C: printf("%04X   INR  C\n",pc); break;
      case 0x0D: printf("%04X   DCR  C\n",pc); break;
      case 0x0E: printf("%04X   MVI  C   #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0x0F: printf("%04X   RRC\n",pc); break;

      case 0x10: printf("%04X   NOP*\n",pc); break;
      case 0x11: printf("%04X   LXI  D   #$%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0x12: printf("%04X   STAX D\n",pc); break;
      case 0x13: printf("%04X   INX  D\n",pc); break;
      case 0x14: printf("%04X   INR  D\n",pc); break;
      case 0x15: printf("%04X   DCR  D\n",pc); break;
      case 0x16: printf("%04X   MVI  D   #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0x17: printf("%04X   RAL\n",pc); break;
      case 0x18: printf("%04X   NOP*\n",pc); break;
      case 0x19: printf("%04X   DAD  D\n",pc); break;
      case 0x1A: printf("%04X   LDAX D\n",pc); break;
      case 0x1B: printf("%04X   DCX  D\n",pc); break;
      case 0x1C: printf("%04X   INR  E\n",pc); break;
      case 0x1D: printf("%04X   DCR  E\n",pc); break;
      case 0x1E: printf("%04X   MVI  E   #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0x1F: printf("%04X   RAR\n",pc); break;

      case 0x20: printf("%04X   NOP*\n",pc); break;
      case 0x21: printf("%04X   LXI  H   #$%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0x22: printf("%04X   SHLD      $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0x23: printf("%04X   INX  H\n",pc); break;
      case 0x24: printf("%04X   INR  H\n",pc); break;
      case 0x25: printf("%04X   DCR  H\n",pc); break;
      case 0x26: printf("%04X   MVI  H   #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0x27: printf("%04X   DAA\n",pc); break;
      case 0x28: printf("%04X   NOP*\n",pc); break;
      case 0x29: printf("%04X   DAD  SP\n",pc); break;
      case 0x2A: printf("%04X   LHLD      $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0x2B: printf("%04X   DCX  H\n",pc); break;
      case 0x2C: printf("%04X   INR  L\n",pc); break;
      case 0x2D: printf("%04X   DCR  L\n",pc); break;
      case 0x2E: printf("%04X   MVI  L   #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0x2F: printf("%04X   CMA\n",pc); break;

      case 0x30: printf("%04X   NOP*\n",pc); break;
      case 0x31: printf("%04X   LXI  SP  #$%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0x32: printf("%04X   STA       $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0x33: printf("%04X   INX  SP\n",pc); break;
      case 0x34: printf("%04X   INR  M\n",pc); break;
      case 0x35: printf("%04X   DCR  M\n",pc); break;
      case 0x36: printf("%04X   MVI  M   #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0x37: printf("%04X   STC\n",pc); break;
      case 0x38: printf("%04X   NOP*\n",pc); break;
      case 0x39: printf("%04X   DAD  SP\n",pc); break;
      case 0x3A: printf("%04X   LDA       $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0x3B: printf("%04X   DCX  SP\n",pc); break;
      case 0x3C: printf("%04X   INR  A\n",pc); break;
      case 0x3D: printf("%04X   DCR  A\n",pc); break;
      case 0x3E: printf("%04X   MVI  A   #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0x3F: printf("%04X   CMC\n",pc); break;

      case 0x40: printf("%04X   MOV  B,B\n",pc); break;
      case 0x41: printf("%04X   MOV  B,C\n",pc); break;
      case 0x42: printf("%04X   MOV  B,D\n",pc); break;
      case 0x43: printf("%04X   MOV  B,E\n",pc); break;
      case 0x44: printf("%04X   MOV  B,H\n",pc); break;
      case 0x45: printf("%04X   MOV  B,L\n",pc); break;
      case 0x46: printf("%04X   MOV  B,M\n",pc); break;
      case 0x47: printf("%04X   MOV  B,A\n",pc); break;
      case 0x48: printf("%04X   MOV  C,B\n",pc); break;
      case 0x49: printf("%04X   MOV  C,C\n",pc); break;
      case 0x4A: printf("%04X   MOV  C,D\n",pc); break;
      case 0x4B: printf("%04X   MOV  C,E\n",pc); break;
      case 0x4C: printf("%04X   MOV  C,H\n",pc); break;
      case 0x4D: printf("%04X   MOV  C,L\n",pc); break;
      case 0x4E: printf("%04X   MOV  C,M\n",pc); break;
      case 0x4F: printf("%04X   MOV  C,A\n",pc); break;

      case 0x50: printf("%04X   MOV  D,B\n",pc); break;
      case 0x51: printf("%04X   MOV  D,C\n",pc); break;
      case 0x52: printf("%04X   MOV  D,D\n",pc); break;
      case 0x53: printf("%04X   MOV  D,E\n",pc); break;
      case 0x54: printf("%04X   MOV  D,H\n",pc); break;
      case 0x55: printf("%04X   MOV  D,L\n",pc); break;
      case 0x56: printf("%04X   MOV  D,M\n",pc); break;
      case 0x57: printf("%04X   MOV  D,A\n",pc); break;
      case 0x58: printf("%04X   MOV  E,B\n",pc); break;
      case 0x59: printf("%04X   MOV  E,C\n",pc); break;
      case 0x5A: printf("%04X   MOV  E,D\n",pc); break;
      case 0x5B: printf("%04X   MOV  E,E\n",pc); break;
      case 0x5C: printf("%04X   MOV  E,H\n",pc); break;
      case 0x5D: printf("%04X   MOV  E,L\n",pc); break;
      case 0x5E: printf("%04X   MOV  E,M\n",pc); break;
      case 0x5F: printf("%04X   MOV  E,A\n",pc); break;

      case 0x60: printf("%04X   MOV  H,B\n",pc); break;
      case 0x61: printf("%04X   MOV  H,C\n",pc); break;
      case 0x62: printf("%04X   MOV  H,D\n",pc); break;
      case 0x63: printf("%04X   MOV  H,E\n",pc); break;
      case 0x64: printf("%04X   MOV  H,H\n",pc); break;
      case 0x65: printf("%04X   MOV  H,L\n",pc); break;
      case 0x66: printf("%04X   MOV  H,M\n",pc); break;
      case 0x67: printf("%04X   MOV  H,A\n",pc); break;
      case 0x68: printf("%04X   MOV  L,B\n",pc); break;
      case 0x69: printf("%04X   MOV  L,C\n",pc); break;
      case 0x6A: printf("%04X   MOV  L,D\n",pc); break;
      case 0x6B: printf("%04X   MOV  L,E\n",pc); break;
      case 0x6C: printf("%04X   MOV  L,H\n",pc); break;
      case 0x6D: printf("%04X   MOV  L,L\n",pc); break;
      case 0x6E: printf("%04X   MOV  L,M\n",pc); break;
      case 0x6F: printf("%04X   MOV  L,A\n",pc); break;

      case 0x70: printf("%04X   MOV  M,B\n",pc); break;
      case 0x71: printf("%04X   MOV  M,C\n",pc); break;
      case 0x72: printf("%04X   MOV  M,D\n",pc); break;
      case 0x73: printf("%04X   MOV  M,E\n",pc); break;
      case 0x74: printf("%04X   MOV  M,H\n",pc); break;
      case 0x75: printf("%04X   MOV  M,L\n",pc); break;
      case 0x76: printf("%04X   HLT\n",pc); break;
      case 0x77: printf("%04X   MOV  M,A\n",pc); break;
      case 0x78: printf("%04X   MOV  A,B\n",pc); break;
      case 0x79: printf("%04X   MOV  A,C\n",pc); break;
      case 0x7A: printf("%04X   MOV  A,D\n",pc); break;
      case 0x7B: printf("%04X   MOV  A,E\n",pc); break;
      case 0x7C: printf("%04X   MOV  A,H\n",pc); break;
      case 0x7D: printf("%04X   MOV  A,L\n",pc); break;
      case 0x7E: printf("%04X   MOV  A,M\n",pc); break;
      case 0x7F: printf("%04X   MOV  A,A\n",pc); break;

      case 0x80: printf("%04X   ADD  B\n",pc); break;
      case 0x81: printf("%04X   ADD  C\n",pc); break;
      case 0x82: printf("%04X   ADD  D\n",pc); break;
      case 0x83: printf("%04X   ADD  E\n",pc); break;
      case 0x84: printf("%04X   ADD  H\n",pc); break;
      case 0x85: printf("%04X   ADD  L\n",pc); break;
      case 0x86: printf("%04X   ADD  M\n",pc); break;
      case 0x87: printf("%04X   ADD  A\n",pc); break;
      case 0x88: printf("%04X   ADC  B\n",pc); break;
      case 0x89: printf("%04X   ADC  C\n",pc); break;
      case 0x8A: printf("%04X   ADC  D\n",pc); break;
      case 0x8B: printf("%04X   ADC  E\n",pc); break;
      case 0x8C: printf("%04X   ADC  H\n",pc); break;
      case 0x8D: printf("%04X   ADC  L\n",pc); break;
      case 0x8E: printf("%04X   ADC  M\n",pc); break;
      case 0x8F: printf("%04X   ADC  A\n",pc); break;

      case 0x90: printf("%04X   SUB  B\n",pc); break;
      case 0x91: printf("%04X   SUB  C\n",pc); break;
      case 0x92: printf("%04X   SUB  D\n",pc); break;
      case 0x93: printf("%04X   SUB  E\n",pc); break;
      case 0x94: printf("%04X   SUB  H\n",pc); break;
      case 0x95: printf("%04X   SUB  L\n",pc); break;
      case 0x96: printf("%04X   SUB  M\n",pc); break;
      case 0x97: printf("%04X   SUB  A\n",pc); break;
      case 0x98: printf("%04X   SBB  B\n",pc); break;
      case 0x99: printf("%04X   SBB  C\n",pc); break;
      case 0x9A: printf("%04X   SBB  D\n",pc); break;
      case 0x9B: printf("%04X   SBB  E\n",pc); break;
      case 0x9C: printf("%04X   SBB  H\n",pc); break;
      case 0x9D: printf("%04X   SBB  L\n",pc); break;
      case 0x9E: printf("%04X   SBB  M\n",pc); break;
      case 0x9F: printf("%04X   SBB  A\n",pc); break;

      case 0xA0: printf("%04X   ANA  B\n",pc); break;
      case 0xA1: printf("%04X   ANA  C\n",pc); break;
      case 0xA2: printf("%04X   ANA  D\n",pc); break;
      case 0xA3: printf("%04X   ANA  E\n",pc); break;
      case 0xA4: printf("%04X   ANA  H\n",pc); break;
      case 0xA5: printf("%04X   ANA  L\n",pc); break;
      case 0xA6: printf("%04X   ANA  M\n",pc); break;
      case 0xA7: printf("%04X   ANA  A\n",pc); break;
      case 0xA8: printf("%04X   XRA  B\n",pc); break;
      case 0xA9: printf("%04X   XRA  C\n",pc); break;
      case 0xAA: printf("%04X   XRA  D\n",pc); break;
      case 0xAB: printf("%04X   XRA  E\n",pc); break;
      case 0xAC: printf("%04X   XRA  H\n",pc); break;
      case 0xAD: printf("%04X   XRA  L\n",pc); break;
      case 0xAE: printf("%04X   XRA  M\n",pc); break;
      case 0xAF: printf("%04X   XRA  A\n",pc); break;

      case 0xB0: printf("%04X   ORA  B\n",pc); break;
      case 0xB1: printf("%04X   ORA  C\n",pc); break;
      case 0xB2: printf("%04X   ORA  D\n",pc); break;
      case 0xB3: printf("%04X   ORA  E\n",pc); break;
      case 0xB4: printf("%04X   ORA  H\n",pc); break;
      case 0xB5: printf("%04X   ORA  L\n",pc); break;
      case 0xB6: printf("%04X   ORA  M\n",pc); break;
      case 0xB7: printf("%04X   ORA  A\n",pc); break;
      case 0xB8: printf("%04X   CMP  B\n",pc); break;
      case 0xB9: printf("%04X   CMP  C\n",pc); break;
      case 0xBA: printf("%04X   CMP  D\n",pc); break;
      case 0xBB: printf("%04X   CMP  E\n",pc); break;
      case 0xBC: printf("%04X   CMP  H\n",pc); break;
      case 0xBD: printf("%04X   CMP  L\n",pc); break;
      case 0xBE: printf("%04X   CMP  M\n",pc); break;
      case 0xBF: printf("%04X   CMP  A\n",pc); break;

      case 0xC0: printf("%04X   RNZ\n",pc); break;
      case 0xC1: printf("%04X   POP  B\n",pc); break;
      case 0xC2: printf("%04X   JNZ       $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xC3: printf("%04X   JMP       $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xC4: printf("%04X   CNZ       $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xC5: printf("%04X   PUSH B\n",pc); break;
      case 0xC6: printf("%04X   ADI      #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0xC7: printf("%04X   RST  0\n",pc); break;
      case 0xC8: printf("%04X   RZ\n",pc); break;
      case 0xC9: printf("%04X   RET\n",pc); break;
      case 0xCA: printf("%04X   JZ        $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xCB: printf("%04X   JMP*      $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xCC: printf("%04X   CZ        $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xCD: printf("%04X   CALL      $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xCE: printf("%04X   ACI      #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0xCF: printf("%04X   RST  1\n",pc); break;

      case 0xD0: printf("%04X   RNC\n",pc); break;
      case 0xD1: printf("%04X   POP  D\n",pc); break;
      case 0xD2: printf("%04X   JNC       $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xD3: printf("%04X   OUT      #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0xD4: printf("%04X   CNC       $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xD5: printf("%04X   PUSH D\n",pc); break;
      case 0xD6: printf("%04X   SUI      #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0xD7: printf("%04X   RST  2\n",pc); break;
      case 0xD8: printf("%04X   RC\n",pc); break;
      case 0xD9: printf("%04X   RET*\n",pc); break;
      case 0xDA: printf("%04X   JC        $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xDB: printf("%04X   IN       #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0xDC: printf("%04X   CC        $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xDD: printf("%04X   CALL*     $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xDE: printf("%04X   SBI      #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0xDF: printf("%04X   RST  3\n",pc); break;

      case 0xE0: printf("%04X   RPO\n",pc); break;
      case 0xE1: printf("%04X   POP  H\n",pc); break;
      case 0xE2: printf("%04X   JPO       $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xE3: printf("%04X   XTHL\n",pc); break;
      case 0xE4: printf("%04X   CPO       $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xE5: printf("%04X   PUSH H\n",pc); break;
      case 0xE6: printf("%04X   ANI      #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0xE7: printf("%04X   RST  4\n",pc); break;
      case 0xE8: printf("%04X   RPE\n",pc); break;
      case 0xE9: printf("%04X   PCHL\n",pc); break;
      case 0xEA: printf("%04X   JPE       $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xEB: printf("%04X   XCHG\n",pc); break;
      case 0xEC: printf("%04X   CPE       $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xED: printf("%04X   CALL*     $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xEE: printf("%04X   XRI      #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0xEF: printf("%04X   RST  5\n",pc); break;

      case 0xF0: printf("%04X   RP\n",pc); break;
      case 0xF1: printf("%04X   POP  PSW\n",pc); break;
      case 0xF2: printf("%04X   JP        $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xF3: printf("%04X   DI\n",pc); break;
      case 0xF4: printf("%04X   CP        $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xF5: printf("%04X   PUSH PSW\n",pc); break;
      case 0xF6: printf("%04X   ORI      #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0xF7: printf("%04X   RST  6\n",pc); break;
      case 0xF8: printf("%04X   RM\n",pc); break;
      case 0xF9: printf("%04X   SPHL\n",pc); break;
      case 0xFA: printf("%04X   JM        $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xFB: printf("%04X   EI\n",pc); break;
      case 0xFC: printf("%04X   CM        $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xFD: printf("%04X   CALL*     $%02X%02X\n", pc, codeBuffer[pc+2], codeBuffer[pc+1]); pc = pc +2; break;
      case 0xFE: printf("%04X   CPI      #$%02X\n",pc, codeBuffer[pc+1]); pc++; break;
      case 0xFF: printf("%04X   RST  7\n",pc); break;





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
  //invaders.h path : C://Users/Joel/Documents/Code/Space_invaders_emu/ROMs/invaders.h
  // Open ROM and fill array with its contents
  fp = fopen("C://Users/Joel/Documents/Code/Space_invaders_emu/ROMs/invaders", "rb");
  fseek(fp, 0, SEEK_END);
  int sizeOfFile = ftell(fp);
  rewind(fp);
  unsigned char buffer[sizeOfFile]; //buffer array which contains binary code
  fread(buffer, sizeOfFile, 1, fp);
  fclose(fp);
  // close file




  //print_code_in_hex(buffer, sizeOfFile);
  printf("\n\nSize: %d bytes\n\n",sizeOfFile);
  dissasemble8080_ROM(buffer, sizeOfFile);

  return 0;
}
