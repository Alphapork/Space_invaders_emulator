#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void stoped_state(){
  //todo
}


void setflags(uint8_t flag_picker, uint8_t operand_1, char operator, uint8_t operand_2, uint8_t *FLAGS) {
  uint8_t result;
  uint8_t parity_count = 0x00;

  if (operator == '+') { //calc result
    result = (operand_1 + operand_2);
  } else if (operator == '-') {
    result = (operand_1 - operand_2);
  } else if (operator == '&') {
    result = (operand_1 & operand_2);
  } else if (operator == '^') {
    result = (operand_1 ^ operand_2);
  } else if (operator == '|') {
    result = (operand_1 | operand_2);
  }

  if ((flag_picker & 0x80) == 0x80) { //Sign flag
    if (result >= 0x80) {
      FLAGS[0] = 0x01;
    } else {
      FLAGS[0] = 0x00;
    }
  }

  if ((flag_picker & 0x40) == 0x40) { //Zero flag
    if (result == 0x00) {
      FLAGS[1] = 0x01;
    } else {
      FLAGS[1] = 0x00;
    }
  }

  if ((flag_picker & 0x10) == 0x10) { // auxilary C flag, checks for carry from 4 Least significant bits
    if (((operand_1 & 0x0F) + (operand_1 & 0x0F)) > 0x0F) {
      FLAGS[2] = 0x01;
    } else if  (((operand_1 & 0x0F) + (operand_1 & 0x0F)) > 0x0F) {
      FLAGS[2] = 0x00;
    }
  }


  // calc nr of 1s in binary nr
  if ((result & 0x01) == 0x01) {
    parity_count = parity_count + 0x01;
  }
  if ((result & 0x02) == 0x02) {
    parity_count = parity_count + 0x01;
  }
  if ((result & 0x04) == 0x04) {
    parity_count = parity_count + 0x01;
  }
  if ((result & 0x08) == 0x08) {
    parity_count = parity_count + 0x01;
  }
  if ((result & 0x10) == 0x10) {
    parity_count = parity_count + 0x01;
  }
  if ((result & 0x20) == 0x20) {
    parity_count = parity_count + 0x01;
  }
  if ((result & 0x40) == 0x40) {
    parity_count = parity_count + 0x01;
  }
  if ((result & 0x80) == 0x80) {
    parity_count = parity_count + 0x01;
  }
  printf("parity_count: %02X\n", parity_count);
  // calc done

  if ((flag_picker & 0x04) == 0x04) { // check if parity_count is even or odd and set p flag accordingly
    if (parity_count % 0x02 == 0) {
      FLAGS[3] = 0x01;
    } else {
      FLAGS[3] = 0x00;
    }
  }


  if ((flag_picker & 0x01) == 0x01) { //carry flag
    if (operator == '+' && (operand_1 + operand_2) > 0xFF ) {   // Carryflag on addition
      FLAGS[4] = 0x01;
    } else if (operator == '+' && (operand_1 + operand_2) < 0xFF) {
      FLAGS[4] = 0x00;
    }

    if (operator == '-' && (operand_1 < operand_2)) { // Carryflag on subraction
      FLAGS[4] = 0x01;
    } else if (operator == '-' && (operand_1 >= operand_2)) {
      FLAGS[4] = 0x00;
    }
  }

}

uint8_t perform_instruction(uint8_t *Memory,uint8_t OP_CODE_0, uint8_t OP_CODE_1, uint8_t OP_CODE_2, uint8_t *B, uint8_t *C, uint8_t *D, uint8_t *E, uint8_t *H, uint8_t *L, uint8_t *A, uint8_t *IN, uint8_t *OUT, uint16_t *SP, uint16_t *PC, uint8_t *FLAGS,uint8_t *ALLOW_INTERUPT, uint8_t *running) {

  uint16_t BC = (*B << 8) + *C;
  uint16_t DE = (*D << 8) + *E;
  uint16_t HL = (*H << 8) + *L;
  uint8_t inc_PC_with = 1; // default, used for all one byte instructions
  uint8_t old_carry = FLAGS[4];
  uint8_t swap;

  // switch case for all OP-Codes
  switch (OP_CODE_0) {
    case 0x00: printf("%04X   NOP\n",*PC); break;

    case 0x01:
      {
        printf("%04X   LXI  B   #$%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        *B = OP_CODE_2;
        *C = OP_CODE_1;
        printf("B: %02X C: %02X\n",*B,*C);
        inc_PC_with = 3;
        break; // store data in BC
      }

    case 0x02:
      {
        printf("%04X   STAX B\n",*PC);
        Memory[BC] = *A;
        printf("Memory[BC]: %02X A: %02X\n",Memory[BC],*A);
        break; // store the value of A in memorycell$BC
      }

    case 0x03:
      {
        printf("%04X   INX  B\n",*PC);
        if (*C == 0xFF) {
          *B = *B + 0x01;
        }
        *C= *C + 0x01;
        printf("B:%02x C:%02X\n",*B,*C);
        break; // incr. BC by one
      }

    case 0x04:
      {
        printf("%04X   INR  B\n",*PC);
        setflags(0xD4,*B, '+', 0x01,FLAGS);
        *B = *B + 0x01;

        break; // incr. B by one
      }

    case 0x05:
      {
        printf("%04X   DCR  B\n",*PC);
        setflags(0xD4,*B, '-', 0x01,FLAGS);
        *B = *B - 0x01;
        break; // decr. B by one
      }

    case 0x06:
      {
        printf("%04X   MVI  B   #$%02X\n",*PC, OP_CODE_1);
        *B = OP_CODE_1;
        inc_PC_with = 2;
        break; // puts the value of the next bit into B
      }

    case 0x07:
      {
        printf("%04X   RLC\n",*PC);
        printf("A before: %02X\n", *A);
        if (*A >= 0x80) {
          *A = (*A << 1) + 0x01;
          FLAGS[4] = 0x01;
        } else {
          *A = (*A << 1);
          FLAGS[4] = 0x00;
        }

        printf("A after: %02X\n", *A);
        printf("C flag: %02X\n", FLAGS[4]);

        break;  //Left bitwise swap
      }

    case 0x08: printf("%04X   NOP*\n",*PC); break;

    case 0x09:
      {

        printf("BC: %04X  HL: %04X  H: %02X  L: %02X\n",BC,HL,*H,*L);
        printf("%04X   DAD  B\n",*PC);
        if ((HL + BC) > 0xFFFF) {
          FLAGS[4] = 0x01;
        }else {
          FLAGS[4] = 0x00;
        }
        HL = HL + BC;
        *H = (HL & 0xFF00) >> 8;
        *L = (HL & 0x00FF);
        printf("HL: %04X  H: %02X  CARRY: %02X\n",HL,*H,FLAGS[4]);


        break;  //adds two 16 bit numbers and loads them in HL, HL = HL + BC, carry flag is set if overflow
      }

    case 0x0A:
      {
        printf("%04X   LDAX B\n",*PC);
        *A = Memory[BC];
        printf("A: %02X  Memory[BC]: %02X\n",*A,Memory[BC]);
        break;
      }   //loads A with data at Memory[BC]

    case 0x0B:
      {
        printf("%04X   DCX  B\n",*PC);
        if (*C == 0x00) {
          *B = *B - 0x01;
        }
        *C= *C - 0x01;
        printf("B:%02x C:%02X\n",*B,*C);
         break;
      } //decr. BC by one

    case 0x0C:
      {
        printf("%04X   INR  C\n",*PC);
        setflags(0xD4,*C, '+', 0x01,FLAGS);
        *C = *C + 0x01;
        break;
      }  // incr. C by one

    case 0x0D:
      {
        printf("%04X   DCR  C\n",*PC);
        setflags(0xD4,*C, '-', 0x01,FLAGS);
        *C = *C - 0x01;
        break; // decr. C by one
      }

    case 0x0E:
      {
        printf("%04X   MVI  C   #$%02X\n",*PC, OP_CODE_1);
        *C = OP_CODE_1;
        inc_PC_with = 2;
        break; // puts the value of the next bit into C
      }

    case 0x0F:
      {
        printf("%04X   RRC\n",*PC);
        printf("A before: %02X\n", *A);
        if (*A % 0x02 != 0) {
          *A = (*A >> 1) + 0x80;
          FLAGS[4] = 0x01;
        } else {
          *A = (*A >> 1);
          FLAGS[4] = 0x00;
        }

        printf("A after: %02X\n", *A);
        printf("C flag: %02X\n", FLAGS[4]);

         break;
      }




      /*********************************************************************/



    case 0x10: printf("%04X   NOP*\n",*PC); break;

    case 0x11:
      {
        printf("%04X   LXI  D   #$%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        *D = OP_CODE_2;
        *E = OP_CODE_1;
        printf("D: %02X E: %02X\n",*D,*E);
        inc_PC_with = 3;
        break; // store data in DE
      }

    case 0x12:
      {
        printf("%04X   STAX D\n",*PC);
        Memory[DE] = *A;
        printf("Memory[DE]: %02X A: %02X\n",Memory[DE],*A);
        break; // store the value of A in memorycell$DE
      }

    case 0x13:
      {
        printf("%04X   INX  D\n",*PC);
        if (*E == 0xFF) {
          *D = *D + 0x01;
        }
        *E = *E + 0x01;
        printf("D:%02x E:%02X\n",*D,*E);
        break; // incr. DE by one
      }

    case 0x14:
      {
        printf("%04X   INR  D\n",*PC);
        setflags(0xD4,*D, '+', 0x01,FLAGS);
        *D = *D + 0x01;
        break; // incr. D by one
      }

    case 0x15:
      {
        printf("%04X   DCR  D\n",*PC);
        setflags(0xD4,*D, '-', 0x01,FLAGS);
        *D = *D - 0x01;
        break; // decr. D by one
      }

    case 0x16:
      {
        printf("%04X   MVI  D   #$%02X\n",*PC, OP_CODE_1);
        *D = OP_CODE_1;
        inc_PC_with = 2;
        break; // puts the value of the next bit into D
      }

    case 0x17:
      {
        printf("%04X   RAL\n",*PC);
        printf("A before: %02X\n", *A);
        if (*A >= 0x80) {
          *A = (*A << 1) + old_carry;
          FLAGS[4] = 0x01;
        } else {
          *A = (*A << 1) + old_carry;
          FLAGS[4] = 0x00;
        }

        printf("A after: %02X\n", *A);
        printf("C flag: %02X\n", FLAGS[4]);

        break;  //Left bitwise swap through carry
      }

    case 0x18: printf("%04X   NOP*\n",*PC); break;

    case 0x19:
      {

        printf("DE: %04X  HL: %04X  H: %02X  L: %02X\n",DE,HL,*H,*L);
        printf("%04X   DAD  D\n",*PC);
        if ((HL + DE) > 0xFFFF) {
          FLAGS[4] = 0x01;
        }else {
          FLAGS[4] = 0x00;
        }
        HL = HL + DE;
        *H = (HL & 0xFF00) >> 8;
        *L = (HL & 0x00FF);
        printf("HL: %04X  H: %02X  CARRY: %02X\n",HL,*H,FLAGS[4]);


        break;  //adds two 16 bit numbers and loads them in HL, HL = HL + DE, carry flag is set if overflow
      }

    case 0x1A:
      {
        printf("%04X   LDAX D\n",*PC);
        *A = Memory[DE];
        printf("A: %02X  Memory[DE]: %02X\n",*A,Memory[DE]);
        break;
      }   //loads A with data at Memory[DE]

    case 0x1B:
      {
        printf("%04X   DCX  D\n",*PC);
        if (*E == 0x00) {
          *D = *D - 0x01;
        }
        *E= *E - 0x01;
        printf("D:%02x E:%02X\n",*D,*E);
         break;
      } //decr. DE by one

    case 0x1C:
      {
        printf("%04X   INR  E\n",*PC);
        setflags(0xD4,*E, '+', 0x01,FLAGS);
        *E = *E + 0x01;
        break;
      }  // incr. E by one

    case 0x1D:
      {
        printf("%04X   DCR  E\n",*PC);
        setflags(0xD4,*E, '-', 0x01,FLAGS);
        *E = *E - 0x01;
        break; // decr. E by one
      }

    case 0x1E:
      {
        printf("%04X   MVI  E   #$%02X\n",*PC, OP_CODE_1);
        *E = OP_CODE_1;
        inc_PC_with = 2;
        break; // puts the value of the next bit into E
      }

    case 0x1F:
      {
        printf("%04X   RAR\n",*PC);
        printf("A before: %02X\n", *A);
        if (*A % 0x02 != 0) {
          *A = (*A >> 1) + (old_carry * 0x80);
          FLAGS[4] = 0x01;
        } else {
          *A = (*A >> 1) + (old_carry * 0x80);
          FLAGS[4] = 0x00;
        }

        printf("A after: %02X\n", *A);
        printf("C flag: %02X\n", FLAGS[4]);

         break;
      }



        /*****************************************************************/




    case 0x20: printf("%04X   NOP*\n",*PC); break;

    case 0x21:
      {
        printf("%04X   LXI  H   #$%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        *H = OP_CODE_2;
        *L = OP_CODE_1;
        printf("H: %02X L: %02X\n",*H,*L);
        inc_PC_with = 3;
        break; // store data in DE
      }

    case 0x22:
      {
        printf("%04X   SHLD     #$%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        Memory[((OP_CODE_2 << 8) + OP_CODE_1)] = *L;
        Memory[((OP_CODE_2 << 8) + OP_CODE_1) + 0x0001] = *H;
        printf("H: %02X L: %02X  Memory[low]: %02X Memory[high]: %02X\n",*H,*L,Memory[((OP_CODE_2 << 8) + OP_CODE_1)],Memory[((OP_CODE_2 << 8) + OP_CODE_1) + 0x0001]);
        inc_PC_with = 3;
        break; // store L and H direct
      }

    case 0x23:
      {
        printf("%04X   INX  H\n",*PC);
        if (*L == 0xFF) {
          *H = *H + 0x01;
        }
        *L = *L + 0x01;
        printf("H:%02x L:%02X\n",*H,*L);
        break; // incr. HL by one
      }

    case 0x24:
      {
        printf("%04X   INR  H\n",*PC);
        setflags(0xD4,*H, '+', 0x01,FLAGS);
        *H = *H + 0x01;
        break; // incr. H by one
      }

    case 0x25:
      {
        printf("%04X   DCR  H\n",*PC);
        setflags(0xD4,*H, '+', 0x01,FLAGS);
        *H = *H - 0x01;
        break; // decr. H by one
      }

    case 0x26:
      {
        printf("%04X   MVI  H   #$%02X\n",*PC, OP_CODE_1);
        *H = OP_CODE_1;
        inc_PC_with = 2;
        break; // puts the value of the next bit into H
      }

    case 0x27:
      {
        printf("%04X   DAA\n",*PC);
        if ( ( (*A & 0x0F) > 0x09) || FLAGS[2]) { //possible ERROR
          *A = *A + 6;
          FLAGS[2] = 0x01;
        } else {
          FLAGS[2] = 0x00;
        }

        if ( (*A > 0x9F) || old_carry) {
          *A = *A + 0x60;
            FLAGS[4] = 0x01;
        } else {
          FLAGS[4] = 0x00;
        }
        break;
      }

    case 0x28: printf("%04X   NOP*\n",*PC); break;

    case 0x29:
      {

        printf("HL: %04X  HL: %04X  H: %02X  L: %02X\n",HL,HL,*H,*L);
        printf("%04X   DAD  H\n",*PC);
        if ((HL + HL) > 0xFFFF) {
          FLAGS[4] = 0x01;
        }else {
          FLAGS[4] = 0x00;
        }
        HL = HL + HL;
        *H = (HL & 0xFF00) >> 8;
        *L = (HL & 0x00FF);
        printf("HL: %04X  H: %02X  CARRY: %02X\n",HL,*H,FLAGS[4]);


        break;  //adds two 16 bit numbers and loads them in HL, HL = HL + HL, carry flag is set if overflow
      }

    case 0x2A:
      {
        printf("%04X   LHLD     #$%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        *L = Memory[((OP_CODE_2 << 8) + OP_CODE_1)];
        *H = Memory[((OP_CODE_2 << 8) + OP_CODE_1) + 0x0001];
        printf("H: %02X L: %02X  Memory[low]: %02X Memory[high]: %02X\n",*H,*L,Memory[((OP_CODE_2 << 8) + OP_CODE_1)],Memory[((OP_CODE_2 << 8) + OP_CODE_1) + 0x0001]);
        inc_PC_with = 3;
        break;
      }   //loads L and H with data at Memory[data and data+1]

    case 0x2B:
      {
        printf("%04X   DCX  H\n",*PC);
        if (*L == 0x00) {
          *H = *H - 0x01;
        }
        *L= *L - 0x01;
        printf("H:%02x L:%02X\n",*H,*L);
         break;
      } //decr. HL by one

    case 0x2C:
      {
        printf("%04X   INR  L\n",*PC);
        setflags(0xD4,*L, '+', 0x01,FLAGS);
        *L = *L + 0x01;
        break;
      }  // incr. L by one

    case 0x2D:
      {
        printf("%04X   DCR  L\n",*PC);
        setflags(0xD4,*L, '-', 0x01,FLAGS);
        *L = *L - 0x01;
        break; // decr. L by one
      }

    case 0x2E:
      {
        printf("%04X   MVI  L   #$%02X\n",*PC, OP_CODE_1);
        *L = OP_CODE_1;
        inc_PC_with = 2;
        break; // puts the value of the next bit into L
      }

    case 0x2F:
      {
        printf("%04X   RRC\n",*PC);
        printf("A before: %02X\n", *A);
        *A = *A ^ 0xFF;
        printf("A after: %02X\n", *A);


         break; //producess 1 complement of A
      }



        /********************************************************************/



    case 0x30: printf("%04X   NOP*\n",*PC); break;

    case 0x31:
      {
        printf("%04X   LXI  SP   #$%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        *SP = (OP_CODE_2 << 8) + OP_CODE_1;
        printf("SP: %04X\n",*SP);
        inc_PC_with = 0x03;
        break; // store data in SP
      }

    case 0x32:
      {
        printf("%04X   STA         $%02X%02X\n",*PC, OP_CODE_2, OP_CODE_1); //ERROR
        Memory[(OP_CODE_2 << 8) + OP_CODE_1] = *A;
        inc_PC_with = 0x03;
        printf("Memory[DATA]: %02X A: %02X\n",Memory[(OP_CODE_2 << 8) + OP_CODE_1],*A);
        break; // store the value of A in memorycell$DATA
      }

    case 0x33:
      {
        printf("%04X   INX  SP\n",*PC);
        *SP = *SP + 0x01;
        printf("SP: %04X\n",*SP);
        break; // incr. SP by one
      }

    case 0x34:
      {
        printf("%04X   INR  M\n",*PC);
        setflags(0xD4, Memory[HL], '+', 0x01,FLAGS);
        Memory[HL] = Memory[HL] + 0x01;
        break; // incr. Memory[HL] by one
      }

    case 0x35:
      {
        printf("%04X   DCR  M\n",*PC);
        setflags(0xD4, Memory[HL], '-', 0x01,FLAGS);
        Memory[HL] = Memory[HL] - 0x01;
        break; // decr. Memory[HL] by one
      }

    case 0x36:
      {
        printf("%04X   MVI  M   #$%02X\n",*PC, OP_CODE_1);
        Memory[HL] = OP_CODE_1;
        inc_PC_with = 2;
        break; // puts the value of the next bit into Memory[HL]
      }

    case 0x37:
      {
        printf("%04X   STC\n",*PC);

          FLAGS[4] = 0x01;
        break;  //Set carry
      }

    case 0x38: printf("%04X   NOP*\n",*PC); break;

    case 0x39:
      {

        printf("SP: %04X  HL: %04X  H: %02X  L: %02X\n",*SP,HL,*H,*L);
        printf("%04X   DAD  SP\n",*PC);
        if ((HL + *SP) > 0xFFFF) {
          FLAGS[4] = 0x01;
        }else {
          FLAGS[4] = 0x00;
        }
        HL = HL + *SP;
        *H = (HL & 0xFF00) >> 8;
        *L = (HL & 0x00FF);
        printf("HL: %04X  H: %02X  CARRY: %02X\n",HL,*H,FLAGS[4]);


        break;  //adds two 16 bit numbers and loads them in HL, HL = HL + SP, carry flag is set if overflow
      }

    case 0x3A:
      {
        printf("%04X   LDA\n",*PC);
        *A = Memory[HL];
        printf("A: %02X  Memory[HL]: %02X\n",*A,Memory[HL]);
        break;
      }   //loads A with data at Memory[HL]

    case 0x3B:
      {
        printf("%04X   DCX  SP\n",*PC);
        *SP = *SP - 0x01;
        printf("D:%02x E:%02X\n",*D,*E);
         break;
      } //decr. SP by one

    case 0x3C:
      {
        printf("%04X   INR  A\n",*PC);
        setflags(0xD4,*A, '+', 0x01, FLAGS);
        *A = *A + 0x01;
        break;
      }  // incr. A by one

    case 0x3D:
      {
        printf("%04X   DCR  A\n",*PC);
        setflags(0xD4,*A, '-', 0x01, FLAGS);
        *A = *A - 0x01;
        break; // decr. A by one
      }

    case 0x3E:
      {
        printf("%04X   MVI  A   #$%02X\n",*PC, OP_CODE_1);
        *A = OP_CODE_1;
        inc_PC_with = 2;
        break; // puts the value of the next bit into A
      }

    case 0x3F:
      {
        printf("C flag: %02X\n", FLAGS[4]);
        printf("%04X   CMC\n",*PC);
        FLAGS[4] = old_carry ^ 0xFF;
        printf("C flag: %02X\n", FLAGS[4]);

         break;
      }





      /*******************************************************************/



    // A bunch of move instructions
    case 0x40: printf("%04X   MOV  B,B\n",*PC); *B = *B ;break;
    case 0x41: printf("%04X   MOV  B,C\n",*PC); *B = *C ;break;
    case 0x42: printf("%04X   MOV  B,D\n",*PC); *B = *D ;break;
    case 0x43: printf("%04X   MOV  B,E\n",*PC); *B = *E ;break;
    case 0x44: printf("%04X   MOV  B,H\n",*PC); *B = *H ;break;
    case 0x45: printf("%04X   MOV  B,L\n",*PC); *B = *L ;break;
    case 0x46: printf("%04X   MOV  B,M\n",*PC); *B = Memory[HL] ;break;
    case 0x47: printf("%04X   MOV  B,A\n",*PC); *B = *A ;break;
    case 0x48: printf("%04X   MOV  C,B\n",*PC); *C = *B ;break;
    case 0x49: printf("%04X   MOV  C,C\n",*PC); *C = *C ;break;
    case 0x4A: printf("%04X   MOV  C,D\n",*PC); *C = *D ;break;
    case 0x4B: printf("%04X   MOV  C,E\n",*PC); *C = *E ;break;
    case 0x4C: printf("%04X   MOV  C,H\n",*PC); *C = *H ;break;
    case 0x4D: printf("%04X   MOV  C,L\n",*PC); *C = *L ;break;
    case 0x4E: printf("%04X   MOV  C,M\n",*PC); *C = Memory[HL] ;break;
    case 0x4F: printf("%04X   MOV  C,A\n",*PC); *C = *A ;break;

    case 0x50: printf("%04X   MOV  D,B\n",*PC); *D = *B ;break;
    case 0x51: printf("%04X   MOV  D,C\n",*PC); *D = *C ;break;
    case 0x52: printf("%04X   MOV  D,D\n",*PC); *D = *D ;break;
    case 0x53: printf("%04X   MOV  D,E\n",*PC); *D = *E ;break;
    case 0x54: printf("%04X   MOV  D,H\n",*PC); *D = *H ;break;
    case 0x55: printf("%04X   MOV  D,L\n",*PC); *D = *L ;break;
    case 0x56: printf("%04X   MOV  D,M\n",*PC); *D = Memory[HL] ;break;
    case 0x57: printf("%04X   MOV  D,A\n",*PC); *D = *A ;break;
    case 0x58: printf("%04X   MOV  E,B\n",*PC); *E = *B ;break;
    case 0x59: printf("%04X   MOV  E,C\n",*PC); *E = *C ;break;
    case 0x5A: printf("%04X   MOV  E,D\n",*PC); *E = *D ;break;
    case 0x5B: printf("%04X   MOV  E,E\n",*PC); *E = *E ;break;
    case 0x5C: printf("%04X   MOV  E,H\n",*PC); *E = *H ;break;
    case 0x5D: printf("%04X   MOV  E,L\n",*PC); *E = *L ;break;
    case 0x5E: printf("%04X   MOV  E,M\n",*PC); *E = Memory[HL] ;break;
    case 0x5F: printf("%04X   MOV  E,A\n",*PC); *E = *A ;break;

    case 0x60: printf("%04X   MOV  H,B\n",*PC); *H = *B ;break;
    case 0x61: printf("%04X   MOV  H,C\n",*PC); *H = *C ;break;
    case 0x62: printf("%04X   MOV  H,D\n",*PC); *H = *D ;break;
    case 0x63: printf("%04X   MOV  H,E\n",*PC); *H = *E ;break;
    case 0x64: printf("%04X   MOV  H,H\n",*PC); *H = *H ;break;
    case 0x65: printf("%04X   MOV  H,L\n",*PC); *H = *L ;break;
    case 0x66: printf("%04X   MOV  H,M\n",*PC); *H = Memory[HL] ;break;
    case 0x67: printf("%04X   MOV  H,A\n",*PC); *H = *A ;break;
    case 0x68: printf("%04X   MOV  L,B\n",*PC); *L = *B ;break;
    case 0x69: printf("%04X   MOV  L,C\n",*PC); *L = *C ;break;
    case 0x6A: printf("%04X   MOV  L,D\n",*PC); *L = *D ;break;
    case 0x6B: printf("%04X   MOV  L,E\n",*PC); *L = *E ;break;
    case 0x6C: printf("%04X   MOV  L,H\n",*PC); *L = *H ;break;
    case 0x6D: printf("%04X   MOV  L,L\n",*PC); *L = *L ;break;
    case 0x6E: printf("%04X   MOV  L,M\n",*PC); *L = Memory[HL] ;break;
    case 0x6F: printf("%04X   MOV  L,A\n",*PC); *L = *A ;break;

    case 0x70: printf("%04X   MOV  M,B\n",*PC); Memory[HL] = *B ;break;
    case 0x71: printf("%04X   MOV  M,C\n",*PC); Memory[HL] = *C ;break;
    case 0x72: printf("%04X   MOV  M,D\n",*PC); Memory[HL] = *D ;break;
    case 0x73: printf("%04X   MOV  M,E\n",*PC); Memory[HL] = *E ;break;
    case 0x74: printf("%04X   MOV  M,H\n",*PC); Memory[HL] = *H ;break;
    case 0x75: printf("%04X   MOV  M,L\n",*PC); Memory[HL] = *L ;break;
    case 0x76: printf("%04X   HLT\n",*PC); *running = 0x00; break; //possible need to switch order to incr. PC before the stopped state
    case 0x77: printf("%04X   MOV  M,A\n",*PC); Memory[HL] = *A ;break;
    case 0x78: printf("%04X   MOV  A,B\n",*PC); *A = *B ;break;
    case 0x79: printf("%04X   MOV  A,C\n",*PC); *A = *C ;break;
    case 0x7A: printf("%04X   MOV  A,D\n",*PC); *A = *D ;break;
    case 0x7B: printf("%04X   MOV  A,E\n",*PC); *A = *E ;break;
    case 0x7C: printf("%04X   MOV  A,H\n",*PC); *A = *H ;break;
    case 0x7D: printf("%04X   MOV  A,L\n",*PC); *A = *L ;break;
    case 0x7E: printf("%04X   MOV  A,M\n",*PC); *A = Memory[HL]; break;
    case 0x7F: printf("%04X   MOV  A,A\n",*PC); *A = *A ;break;




    /********************************************************************/




    // A bunch of addition instructions


    case 0x80: printf("%04X   ADD  B\n",*PC); setflags(0xD5, *A, '+', *B, FLAGS); *A = *A + *B; break;
    case 0x81: printf("%04X   ADD  C\n",*PC); setflags(0xD5, *A, '+', *C, FLAGS); *A = *A + *C; break;
    case 0x82: printf("%04X   ADD  D\n",*PC); setflags(0xD5, *A, '+', *D, FLAGS); *A = *A + *D; break;
    case 0x83: printf("%04X   ADD  E\n",*PC); setflags(0xD5, *A, '+', *E, FLAGS); *A = *A + *E; break;
    case 0x84: printf("%04X   ADD  H\n",*PC); setflags(0xD5, *A, '+', *H, FLAGS); *A = *A + *H; break;
    case 0x85: printf("%04X   ADD  L\n",*PC); setflags(0xD5, *A, '+', *L, FLAGS); *A = *A + *L; break;
    case 0x86: printf("%04X   ADD  M\n",*PC); setflags(0xD5, *A, '+', Memory[HL], FLAGS); *A = *A + Memory[HL]; break;
    case 0x87: printf("%04X   ADD  A\n",*PC); setflags(0xD5, *A, '+', *A, FLAGS); *A = *A + *A; break;
    case 0x88: printf("%04X   ADC  B\n",*PC); setflags(0xD5, *A, '+', *B + old_carry, FLAGS); *A = *A + *B + old_carry; break;
    case 0x89: printf("%04X   ADC  C\n",*PC); setflags(0xD5, *A, '+', *C + old_carry, FLAGS); *A = *A + *C + old_carry; break;
    case 0x8A: printf("%04X   ADC  D\n",*PC); setflags(0xD5, *A, '+', *D + old_carry, FLAGS); *A = *A + *D + old_carry; break;
    case 0x8B: printf("%04X   ADC  E\n",*PC); setflags(0xD5, *A, '+', *E + old_carry, FLAGS); *A = *A + *E + old_carry; break;
    case 0x8C: printf("%04X   ADC  H\n",*PC); setflags(0xD5, *A, '+', *H + old_carry, FLAGS); *A = *A + *H + old_carry; break;
    case 0x8D: printf("%04X   ADC  L\n",*PC); setflags(0xD5, *A, '+', *L + old_carry, FLAGS); *A = *A + *L + old_carry; break;
    case 0x8E: printf("%04X   ADC  M\n",*PC); setflags(0xD5, *A, '+', Memory[HL] + old_carry, FLAGS); *A = *A + Memory[HL] + old_carry; break;
    case 0x8F: printf("%04X   ADC  A\n",*PC); setflags(0xD5, *A, '+', *A + old_carry, FLAGS); *A = *A + *A + old_carry; break;



    // A bunch os subtraction instructions
    case 0x90: printf("%04X   SUB  B\n",*PC); setflags(0xD5, *A, '-', *B, FLAGS); *A = *A - *B; break;
    case 0x91: printf("%04X   SUB  C\n",*PC); setflags(0xD5, *A, '-', *C, FLAGS); *A = *A - *C; break;
    case 0x92: printf("%04X   SUB  D\n",*PC); setflags(0xD5, *A, '-', *D, FLAGS); *A = *A - *D; break;
    case 0x93: printf("%04X   SUB  E\n",*PC); setflags(0xD5, *A, '-', *E, FLAGS); *A = *A - *E; break;
    case 0x94: printf("%04X   SUB  H\n",*PC); setflags(0xD5, *A, '-', *H, FLAGS); *A = *A - *H; break;
    case 0x95: printf("%04X   SUB  L\n",*PC); setflags(0xD5, *A, '-', *L, FLAGS); *A = *A - *L; break;
    case 0x96: printf("%04X   SUB  M\n",*PC); setflags(0xD5, *A, '-', Memory[HL], FLAGS); *A = *A - Memory[HL]; break;
    case 0x97: printf("%04X   SUB  A\n",*PC); setflags(0xD5, *A, '-', *A, FLAGS); *A = *A - *A; break;
    case 0x98: printf("%04X   SBB  B\n",*PC); setflags(0xD5, *A, '-', *B + old_carry, FLAGS); *A = *A - *B + old_carry; break;
    case 0x99: printf("%04X   SBB  C\n",*PC); setflags(0xD5, *A, '-', *C + old_carry, FLAGS); *A = *A - *C + old_carry; break;
    case 0x9A: printf("%04X   SBB  D\n",*PC); setflags(0xD5, *A, '-', *D + old_carry, FLAGS); *A = *A - *D + old_carry; break;
    case 0x9B: printf("%04X   SBB  E\n",*PC); setflags(0xD5, *A, '-', *E + old_carry, FLAGS); *A = *A - *E + old_carry; break;
    case 0x9C: printf("%04X   SBB  H\n",*PC); setflags(0xD5, *A, '-', *H + old_carry, FLAGS); *A = *A - *H + old_carry; break;
    case 0x9D: printf("%04X   SBB  L\n",*PC); setflags(0xD5, *A, '-', *L + old_carry, FLAGS); *A = *A - *L + old_carry; break;
    case 0x9E: printf("%04X   SBB  M\n",*PC); setflags(0xD5, *A, '-', Memory[HL] + old_carry, FLAGS); *A = *A - Memory[HL] + old_carry; break;
    case 0x9F: printf("%04X   SBB  A\n",*PC); setflags(0xD5, *A, '-', *A + old_carry, FLAGS); *A = *A - *A + old_carry; break;





    // A bunch of logical instructions



    case 0xA0: printf("%04X   ANA  B\n",*PC); setflags(0xD4, *A, '&', *B, FLAGS); *A = *A & *B; FLAGS[4] = 0x01; break;
    case 0xA1: printf("%04X   ANA  C\n",*PC); setflags(0xD4, *A, '&', *C, FLAGS); *A = *A & *C; FLAGS[4] = 0x01; break;
    case 0xA2: printf("%04X   ANA  D\n",*PC); setflags(0xD4, *A, '&', *D, FLAGS); *A = *A & *D; FLAGS[4] = 0x01; break;
    case 0xA3: printf("%04X   ANA  E\n",*PC); setflags(0xD4, *A, '&', *E, FLAGS); *A = *A & *E; FLAGS[4] = 0x01; break;
    case 0xA4: printf("%04X   ANA  H\n",*PC); setflags(0xD4, *A, '&', *H, FLAGS); *A = *A & *H; FLAGS[4] = 0x01; break;
    case 0xA5: printf("%04X   ANA  L\n",*PC); setflags(0xD4, *A, '&', *L, FLAGS); *A = *A & *L; FLAGS[4] = 0x01; break;
    case 0xA6: printf("%04X   ANA  M\n",*PC); setflags(0xD4, *A, '&', Memory[HL], FLAGS); *A = *A & Memory[HL]; FLAGS[4] = 0x01; break;
    case 0xA7: printf("%04X   ANA  A\n",*PC); setflags(0xD4, *A, '&', *A, FLAGS); *A = *A & *A; FLAGS[4] = 0x01; break;
    case 0xA8: printf("%04X   XRA  B\n",*PC); setflags(0xD4, *A, '^', *B, FLAGS); *A = *A ^ *B; FLAGS[4] = 0x00; break;
    case 0xA9: printf("%04X   XRA  C\n",*PC); setflags(0xD4, *A, '^', *C, FLAGS); *A = *A ^ *C; FLAGS[4] = 0x00; break;
    case 0xAA: printf("%04X   XRA  D\n",*PC); setflags(0xD4, *A, '^', *D, FLAGS); *A = *A ^ *D; FLAGS[4] = 0x00; break;
    case 0xAB: printf("%04X   XRA  E\n",*PC); setflags(0xD4, *A, '^', *E, FLAGS); *A = *A ^ *E; FLAGS[4] = 0x00; break;
    case 0xAC: printf("%04X   XRA  H\n",*PC); setflags(0xD4, *A, '^', *H, FLAGS); *A = *A ^ *H; FLAGS[4] = 0x00; break;
    case 0xAD: printf("%04X   XRA  L\n",*PC); setflags(0xD4, *A, '^', *L, FLAGS); *A = *A ^ *L; FLAGS[4] = 0x00; break;
    case 0xAE: printf("%04X   XRA  M\n",*PC); setflags(0xD4, *A, '^', Memory[HL], FLAGS); *A = *A ^ Memory[HL]; FLAGS[4] = 0x00; break;
    case 0xAF: printf("%04X   XRA  A\n",*PC); setflags(0xD4, *A, '^', *A, FLAGS); *A = *A ^ *A; FLAGS[4] = 0x00; break;


    case 0xB0: printf("%04X   ORA  B\n",*PC); setflags(0xD4, *A, '|', *B, FLAGS); *A = *A | *B; FLAGS[4] = 0x00; break;
    case 0xB1: printf("%04X   ORA  C\n",*PC); setflags(0xD4, *A, '|', *C, FLAGS); *A = *A | *C; FLAGS[4] = 0x00; break;
    case 0xB2: printf("%04X   ORA  D\n",*PC); setflags(0xD4, *A, '|', *D, FLAGS); *A = *A | *D; FLAGS[4] = 0x00; break;
    case 0xB3: printf("%04X   ORA  E\n",*PC); setflags(0xD4, *A, '|', *E, FLAGS); *A = *A | *E; FLAGS[4] = 0x00; break;
    case 0xB4: printf("%04X   ORA  H\n",*PC); setflags(0xD4, *A, '|', *H, FLAGS); *A = *A | *H; FLAGS[4] = 0x00; break;
    case 0xB5: printf("%04X   ORA  L\n",*PC); setflags(0xD4, *A, '|', *L, FLAGS); *A = *A | *L; FLAGS[4] = 0x00; break;
    case 0xB6: printf("%04X   ORA  M\n",*PC); setflags(0xD4, *A, '|', Memory[HL], FLAGS); *A = *A | Memory[HL]; FLAGS[4] = 0x00; break;
    case 0xB7: printf("%04X   ORA  A\n",*PC); setflags(0xD4, *A, '|', *A, FLAGS); break;
    case 0xB8: printf("%04X   CMP  B\n",*PC); setflags(0xD4, *A, '-', *B, FLAGS); break;
    case 0xB9: printf("%04X   CMP  C\n",*PC); setflags(0xD4, *A, '-', *C, FLAGS); break;
    case 0xBA: printf("%04X   CMP  D\n",*PC); setflags(0xD4, *A, '-', *D, FLAGS); break;
    case 0xBB: printf("%04X   CMP  E\n",*PC); setflags(0xD4, *A, '-', *E, FLAGS); break;
    case 0xBC: printf("%04X   CMP  H\n",*PC); setflags(0xD4, *A, '-', *H, FLAGS); break;
    case 0xBD: printf("%04X   CMP  L\n",*PC); setflags(0xD4, *A, '-', *L, FLAGS); break;
    case 0xBE: printf("%04X   CMP  M\n",*PC); setflags(0xD4, *A, '-', Memory[HL], FLAGS); break;
    case 0xBF: printf("%04X   CMP  A\n",*PC); setflags(0xD4, *A, '-', *A, FLAGS); break;


    /************************************************************************/



    case 0xC0: //return on no zero
    {
      printf("%04X   RNZ\n",*PC);
      if (!FLAGS[1]) {
        inc_PC_with = 0x00;
        *PC = (Memory[*SP + 0x01] << 8) + Memory[*SP];
        *SP = *SP + 0x02;
      } else {
        printf("No return, z flag is 1\n");
      }
      break;
    }

    case 0xC1: // pop 2 bytes of the stack and put them into B and C registers
      {
        printf("%04X   POP  B\n",*PC);
        *B = Memory[*SP + 0x01];
        *C = Memory[*SP];
        *SP = *SP + 0x02;
        break;
      }

    case 0xC2: // Jump on no zero
      {
        printf("%04X   JNZ       $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (!FLAGS[1]) {
          inc_PC_with = 0x00;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No jump, z flag is 1\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xC3: //Jump unconditionally
      {
        printf("%04X   JMP       $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        inc_PC_with = 0x00;
        *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        break;

      }
    case 0xC4: // Call subroutine on no zero
      {
        printf("%04X   CNZ       $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (!FLAGS[1]) {
          inc_PC_with = 0x00;
          Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
          Memory[*SP - 0x02] = *PC & 0x00FF;
          *SP = *SP - 0x02;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No Call, z flag is 1\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xC5: // push contents of B and C registers onto the stack
      {
        printf("%04X   PUSH B\n",*PC);
        Memory[*SP - 0x01] = *B;
        Memory[*SP - 0x02] = *C;
        *SP = *SP - 0x02;
        break;
      }

    case 0xC6: //add data to A register
      {
        printf("%04X   ADI      #$%02X\n",*PC, OP_CODE_1);
        inc_PC_with = 0x02;
        setflags(0xD5, *A, '+', OP_CODE_1, FLAGS);
        *A = *A + OP_CODE_1;
        break;
      }

    case 0xC7: //reset to adress 0x0000
      {
        printf("%04X   RST  0\n",*PC);
        inc_PC_with = 0x00;
        Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
        Memory[*SP - 0x02] = *PC & 0x00FF;
        *SP = *SP - 0x02;
        *PC = 0x0000;
        break;
      }

    case 0xC8: //return on zero
      {
        printf("%04X   RZ\n",*PC);
        if (FLAGS[1]) {
          inc_PC_with = 0x00;
          *PC = (Memory[*SP + 0x01] << 8) + Memory[*SP];
          *SP = *SP + 0x02;
        } else {
          printf("No return, z flag is 0\n");
        }
        break;
      }

    case 0xC9: // return unconditionally
      {
        printf("%04X   RET\n",*PC);
          inc_PC_with = 0x00;
          *PC = (Memory[*SP + 0x01] << 8) + Memory[*SP];
          *SP = *SP + 0x02;
        break;
      }

    case 0xCA: // jump on zero
      {
        printf("%04X   JZ        $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (FLAGS[1]) {
          inc_PC_with = 0x00;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No jump, z flag is 0\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xCB: // Jump unconditionally
    {
      printf("%04X   JMP*      $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
      inc_PC_with = 0x00;
      *PC = (OP_CODE_2 << 8) + OP_CODE_1;
      break;
    }

    case 0xCC:
    {
        printf("%04X   CZ       $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (FLAGS[1]) {
          inc_PC_with = 0x00;
          Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
          Memory[*SP - 0x02] = *PC & 0x00FF;
          *SP = *SP - 0x02;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No Call, z flag is 0\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xCD: // Call unconditionally
      {
        if (5 ==  ((OP_CODE_2 << 8) | OP_CODE_1)) {
            if (*C == 9) {
              uint16_t offset = (*D<<8) | (*E);
              char *str = &Memory[offset+3];  //skip the prefix bytes
              while (*str != '$')
                printf("%c", *str++);
              printf("\n");
            }  else if (*C == 2) {
              //saw this in the inspected code, never saw it called
              printf ("print char routine called\n");
            }
        } else if (0 ==  ((OP_CODE_2 << 8) | OP_CODE_1))    {
          exit(0);
          }



          printf("%04X   CALL       $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
          inc_PC_with = 0x00;
          Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
          Memory[*SP - 0x02] = *PC & 0x00FF;
          *SP = *SP - 0x02;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
          break;
      }

    case 0xCE:
      {
        printf("%04X   ACI      #$%02X\n",*PC, OP_CODE_1);
        inc_PC_with = 0x02;
        setflags(0xD5, *A, '+', OP_CODE_1 + old_carry, FLAGS);
        *A = *A + OP_CODE_1 + old_carry;
        break;
      }

    case 0xCF: // resets PC to address 0x0008
      {
        printf("%04X   RST  1\n",*PC);
        inc_PC_with = 0x00;
        Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
        Memory[*SP - 0x02] = *PC & 0x00FF;
        *SP = *SP - 0x02;
        *PC = 0x0008;
        break;
      }


      /*******************************************************************/



    case 0xD0: //return on no carry
    {
      printf("%04X   RNC\n",*PC);
      if (!old_carry) {
        inc_PC_with = 0x00;
        *PC = (Memory[*SP + 0x01] << 8) + Memory[*SP];
        *SP = *SP + 0x02;
      } else {
        printf("No return, c flag is 1\n");
      }
      break;
    }

    case 0xD1: // pop 2 bytes of the stack and put them into D and E registers
      {
        printf("%04X   POP  D\n",*PC);
        *D = Memory[*SP + 0x01];
        *E = Memory[*SP];
        *SP = *SP + 0x02;
        break;
      }

    case 0xD2: // Jump on no carry
      {
        printf("%04X   JNC       $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (!old_carry) {
          inc_PC_with = 0x00;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No jump, c flag is 1\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xD3: //Set Output device nr DATA to the same value as register A
      {
        printf("%04X   OUT       $%02X\n", *PC, OP_CODE_1);
        OUT[OP_CODE_1] = *A;
        inc_PC_with = 0x02;
        break;

      }
    case 0xD4: // Call subroutine on no carry
      {
        printf("%04X   CNC       $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (!old_carry) {
          inc_PC_with = 0x00;
          Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
          Memory[*SP - 0x02] = *PC & 0x00FF;
          *SP = *SP - 0x02;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No Call, c flag is 1\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xD5: // push contents of D and E registers onto the stack
      {
        printf("%04X   PUSH D\n",*PC);
        Memory[*SP - 0x01] = *D;
        Memory[*SP - 0x02] = *E;
        *SP = *SP - 0x02;
        break;
      }

    case 0xD6: //subtract data from A register
      {
        printf("%04X   SUI      #$%02X\n",*PC, OP_CODE_1);
        inc_PC_with = 0x02;
        setflags(0xD5, *A, '-', OP_CODE_1, FLAGS);
        *A = *A - OP_CODE_1;
        break;
      }

    case 0xD7: //reset to adress 0x0010
      {
        printf("%04X   RST  2\n",*PC);
        inc_PC_with = 0x00;
        Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
        Memory[*SP - 0x02] = *PC & 0x00FF;
        *SP = *SP - 0x02;
        *PC = 0x0010;
        break;
      }

    case 0xD8: //return on carry
      {
        printf("%04X   RC\n",*PC);
        if (old_carry) {
          inc_PC_with = 0x00;
          *PC = (Memory[*SP + 0x01] << 8) + Memory[*SP];
          *SP = *SP + 0x02;
        } else {
          printf("No return, c flag is 0\n");
        }
        break;
      }

    case 0xD9: // return unconditionally
      {
        printf("%04X   RET*\n",*PC);
          inc_PC_with = 0x00;
          *PC = (Memory[*SP + 0x01] << 8) + Memory[*SP];
          *SP = *SP + 0x02;
        break;
      }

    case 0xDA: // jump on carry
      {
        printf("%04X   JC        $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (old_carry) {
          inc_PC_with = 0x00;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No jump, c flag is 0\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xDB: // Read contents of IN[DATA] into A register
    {
    printf("%04X   IN        $%02X\n", *PC, OP_CODE_1);
    *A = OUT[OP_CODE_1];
    inc_PC_with = 0x02;
    break;
    }

    case 0xDC:
    {
        printf("%04X   CC       $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (old_carry) {
          inc_PC_with = 0x00;
          Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
          Memory[*SP - 0x02] = *PC & 0x00FF;
          *SP = *SP - 0x02;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No Call, c flag is 0\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xDD: // Call unconditionally
      {
          printf("%04X   CALL*      $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
          inc_PC_with = 0x00;
          Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
          Memory[*SP - 0x02] = *PC & 0x00FF;
          *SP = *SP - 0x02;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
          break;
      }

    case 0xDE: // subtract with borrow
      {
        printf("%04X   SBI      #$%02X\n",*PC, OP_CODE_1);
        inc_PC_with = 0x02;
        setflags(0xD5, *A, '-', OP_CODE_1 + old_carry, FLAGS);
        *A = *A - (OP_CODE_1 + old_carry);
        break;
      }

    case 0xDF: // resets PC to address 0x0018
      {
        printf("%04X   RST  3\n",*PC);
        inc_PC_with = 0x00;
        Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
        Memory[*SP - 0x02] = *PC & 0x00FF;
        *SP = *SP - 0x02;
        *PC = 0x0018;
        break;
      }



      /***********************************************************************/


    case 0xE0: //return on parity odd
    {
      printf("%04X   RPO\n",*PC);
      if (!FLAGS[3]) {
        inc_PC_with = 0x00;
        *PC = (Memory[*SP + 0x01] << 8) + Memory[*SP];
        *SP = *SP + 0x02;
      } else {
        printf("No return, p flag is 1\n");
      }
      break;
    }

    case 0xE1: // pop 2 bytes of the stack and put them into H and L registers
      {
        printf("%04X   POP  H\n",*PC);
        *H = Memory[*SP + 0x01];
        *L = Memory[*SP];
        *SP = *SP + 0x02;
        break;
      }

    case 0xE2: // Jump on parity odd
      {
        printf("%04X   JPO       $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (!FLAGS[3]) {
          inc_PC_with = 0x00;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No jump, p flag is 1\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xE3: //Exchange contents of SP and SP + 1 with H and L registers
      {
        printf("%04X   XTHL\n", *PC, OP_CODE_1);
        swap = *L;
        *L = Memory[*SP];
        Memory[*SP] = swap;
        swap = *H;
        *H = Memory[*SP + 1];
        Memory[*SP + 1] = swap;
        break;
      }

    case 0xE4: // Call subroutine on parity odd
      {
        printf("%04X   CPO       $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (!FLAGS[3]) {
          inc_PC_with = 0x00;
          Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
          Memory[*SP - 0x02] = *PC & 0x00FF;
          *SP = *SP - 0x02;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No Call, c flag is 1\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xE5: // push contents of H and L registers onto the stack
      {
        printf("%04X   PUSH H\n",*PC);
        Memory[*SP - 0x01] = *H;
        Memory[*SP - 0x02] = *L;
        *SP = *SP - 0x02;
        break;
      }

    case 0xE6: //logicaly & data and A register
      {
        printf("%04X   ANI      #$%02X\n",*PC, OP_CODE_1);
        inc_PC_with = 0x02;
        setflags(0xD4, *A, '&', OP_CODE_1, FLAGS);
        FLAGS[4] = 0x00;
        *A = (*A & OP_CODE_1);
        break;
      }

    case 0xE7: //reset to adress 0x0020
      {
        printf("%04X   RST  4\n",*PC);
        inc_PC_with = 0x00;
        Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
        Memory[*SP - 0x02] = *PC & 0x00FF;
        *SP = *SP - 0x02;
        *PC = 0x0020;
        break;
      }

    case 0xE8: //return on parity even
      {
        printf("%04X   RPE\n",*PC);
        if (FLAGS[3]) {
          inc_PC_with = 0x00;
          *PC = (Memory[*SP + 0x01] << 8) + Memory[*SP];
          *SP = *SP + 0x02;
        } else {
          printf("No return, p flag is 0\n");
        }
        break;
      }

    case 0xE9: // put value of HL into PC
      {
        printf("%04X   PCHL\n",*PC);
        *PC = HL;
        inc_PC_with = 0x00;
        break;
      }

    case 0xEA: // jump on parity even
      {
        printf("%04X   JPE       $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (FLAGS[3]) {
          inc_PC_with = 0x00;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No jump, p flag is 0\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xEB: // Swap HL and DE
    {
    printf("%04X   XCHG\n", *PC);
    swap = *H;
    *H = *D;
    *D = swap;
    swap = *L;
    *L = *E;
    *E = swap;
    break;
    }

    case 0xEC: // call on parity even
    {
        printf("%04X   CPE      $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (FLAGS[3]) {
          inc_PC_with = 0x00;
          Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
          Memory[*SP - 0x02] = *PC & 0x00FF;
          *SP = *SP - 0x02;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No Call, p flag is 0\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xED: // Call unconditionally
      {
          printf("%04X   CALL*      $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
          inc_PC_with = 0x00;
          Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
          Memory[*SP - 0x02] = *PC & 0x00FF;
          *SP = *SP - 0x02;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
          break;
      }

    case 0xEE: // xor data and A register
      {
        printf("%04X   XRI      #$%02X\n",*PC, OP_CODE_1);
        inc_PC_with = 0x02;
        setflags(0xD5, *A, '^', OP_CODE_1, FLAGS);
        *A = *A ^ OP_CODE_1;
        break;
      }

    case 0xEF: // resets PC to address 0x0028
      {
        printf("%04X   RST  5\n",*PC);
        inc_PC_with = 0x00;
        Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
        Memory[*SP - 0x02] = *PC & 0x00FF;
        *SP = *SP - 0x02;
        *PC = 0x0028;
        break;
      }


    /*************************************************************************/


    case 0xF0: //return on plus
    {
      printf("%04X   RP\n",*PC);
      if (!FLAGS[0]) {
        inc_PC_with = 0x00;
        *PC = (Memory[*SP + 0x01] << 8) + Memory[*SP];
        *SP = *SP + 0x02;
      } else {
        printf("No return, s flag is 1\n");
      }
      break;
    }

    case 0xF1: // pop 2 bytes of the stack and set flags and register A accordingly
      {
        printf("%04X   POP  PSW\n",*PC);

        FLAGS[0] = (Memory[*SP + 0x01] & 0x80) >> 7;
        FLAGS[1] = (Memory[*SP + 0x01] & 0x40) >> 6;
        FLAGS[2] = (Memory[*SP + 0x01] & 0x10) >> 4;
        FLAGS[3] = (Memory[*SP + 0x01] & 0x04) >> 2;
        FLAGS[4] = (Memory[*SP + 0x01] & 0x01);
        *A = Memory[*SP];
        *SP = *SP + 0x02;
        break;
      }

    case 0xF2: // Jump on plus
      {
        printf("%04X   JP       $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (!FLAGS[0]) {
          inc_PC_with = 0x00;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No jump, s flag is 1\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xF3: //Disable interupts
      {
        printf("%04X   DI\n", *PC);
        *ALLOW_INTERUPT = 0x00;
        break;
      }

    case 0xF4: // Call subroutine on plus
      {
        printf("%04X   CP       $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (!FLAGS[0]) {
          inc_PC_with = 0x00;
          Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
          Memory[*SP - 0x02] = *PC & 0x00FF;
          *SP = *SP - 0x02;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No Call, s flag is 1\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xF5: // push current vallue of A and flags to stack
      {
        printf("%04X   PUSH PSW\n",*PC);
        uint8_t flagbyte = (FLAGS[0] << 7) + (FLAGS[1] << 6) + (FLAGS[2] << 4) + (FLAGS[3] << 2) + FLAGS[0];
        Memory[*SP - 0x01] = flagbyte;
        Memory[*SP - 0x02] = *A;
        *SP = *SP - 0x02;
        break;
      }

    case 0xF6: //logicaly or data and A register
      {
        printf("%04X   ORI      #$%02X\n",*PC, OP_CODE_1);
        inc_PC_with = 0x02;
        setflags(0xD5, *A, '|', OP_CODE_1, FLAGS);
        *A = *A | OP_CODE_1;
        break;
      }

    case 0xF7: //reset to adress 0x0030
      {
        printf("%04X   RST  6\n",*PC);
        inc_PC_with = 0x00;
        Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
        Memory[*SP - 0x02] = *PC & 0x00FF;
        *SP = *SP - 0x02;
        *PC = 0x0030;
        break;
      }

    case 0xF8: //return on minus
      {
        printf("%04X   RM\n",*PC);
        if (FLAGS[0]) {
          inc_PC_with = 0x00;
          *PC = (Memory[*SP + 0x01] << 8) + Memory[*SP];
          *SP = *SP + 0x02;
        } else {
          printf("No return, s flag is 0\n");
        }
        break;
      }

    case 0xF9: // put value of HL into SP
      {
        printf("%04X   SPHL\n",*PC);
        *SP = HL;
        break;
      }

    case 0xFA: // jump on minus
      {
        printf("%04X   JM        $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (FLAGS[0]) {
          inc_PC_with = 0x00;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No jump, p flag is 0\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xFB: // Enable interupts
    {
    printf("%04X   EI\n", *PC);
    *ALLOW_INTERUPT = 0x01;
    break;
    }

    case 0xFC: // call on minus
    {
        printf("%04X   CM       $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
        if (FLAGS[0]) {
          inc_PC_with = 0x00;
          Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
          Memory[*SP - 0x02] = *PC & 0x00FF;
          *SP = *SP - 0x02;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
        } else {
          printf("No Call, s flag is 0\n");
          inc_PC_with = 0x03;
        }
        break;
      }

    case 0xFD: // Call unconditionally
      {
          printf("%04X   CALL*      $%02X%02X\n", *PC, OP_CODE_2, OP_CODE_1);
          inc_PC_with = 0x00;
          Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
          Memory[*SP - 0x02] = *PC & 0x00FF;
          *SP = *SP - 0x02;
          *PC = (OP_CODE_2 << 8) + OP_CODE_1;
          break;
      }

    case 0xFE: // compare data and A register
      {
        printf("%04X   CPI      #$%02X\n",*PC, OP_CODE_1);
        inc_PC_with = 0x02;
        setflags(0xD5, *A, '-', OP_CODE_1, FLAGS);
        break;
      }

    case 0xFF: // resets PC to address 0x0038
      {
        printf("%04X   RST  7\n",*PC);
        inc_PC_with = 0x00;
        Memory[*SP - 0x01] = (*PC & 0xFF00) >> 8;
        Memory[*SP - 0x02] = *PC & 0x00FF;
        *SP = *SP - 0x02;
        *PC = 0x0038;
        break;
      }



  }


  return inc_PC_with;
}





int main(int argc, char **argv) {
  //init memory and registers
  uint8_t B = 0x00, C = 0x00, D = 0x00, E = 0x00, H = 0x00, L = 0x00, A = 0x00;
  uint16_t SP = 0x0000, PC = 0x0100;
  uint8_t Memory[0x4000];
  //init flags, 0:S, 1:Z, 2:A, 3:P, 4:C
  uint8_t FLAGS[5];
  uint8_t OUT[0xFF]; // output
  uint8_t IN[0xFF]; // input
  uint8_t ALLOW_INTERUPT = 0x01;
  uint8_t running = 0x01;

  // Open ROM and fill array with its contents
  FILE *fp;
  //invaders path : C://Users/Joel/Documents/Code/Space_invaders_emu/ROMs/invaders
  fp = fopen(argv[1], "rb");
  fseek(fp, 0, SEEK_END);
  int size_of_ROM = ftell(fp);
  rewind(fp);
  uint8_t ROM_code_buffer[size_of_ROM]; //buffer array which contains binary code
  fread(ROM_code_buffer, size_of_ROM, 1, fp);
  fclose(fp);   // close file
  //fill memory with ROM
  for (int i = 0; i < size_of_ROM; i++) {
    Memory[i] = ROM_code_buffer[i];
  }
  while (running) {
    printf("OP-code: %02X\n",Memory[PC]);
    PC += perform_instruction(Memory, Memory[PC], Memory[PC+1], Memory[PC+2], &B, &C, &D, &E, &H, &L, &A, IN, OUT, &SP, &PC, FLAGS, &ALLOW_INTERUPT, &running);
    printf("A: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X M[HL]: %02X SP: %04X PC: %04X \n",A,B,C,D,E,H,L,Memory[(H << 8) + L],SP,PC); //printing all the registers and memory att HL
    printf("Sign flag: %02X Zero flag: %02X Auxilary_C flag: %02X Pairity flag: %02X Carry flag: %02X\n",FLAGS[0],FLAGS[1],FLAGS[2],FLAGS[3],FLAGS[4] );

      getchar();

  }








  return 0;
}
