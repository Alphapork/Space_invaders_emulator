#ifndef _SPACE_INVADERS_SDL
#define _SPACE_INVADERS_SDL

typedef struct space_invaders_machine {
  uint8_t Memory[0xFFFF];
  e8080_cpu CPU;
  int width;
  int height;

  //IO
  uint8_t OUT_PORT2;  //shift amount (3 bits)
  uint8_t OUT_PORT3;  //sound bits
  uint8_t OUT_PORT4; //shift data
  uint8_t OUT_PORT5; //sound bits
  uint8_t OUT_PORT6; //watchdog

  uint8_t IN_PORT0; //inputs
  uint8_t IN_PORT1; //inputs
  uint8_t IN_PORT2; //inputs
  uint8_t IN_PORT3; //bit shift register read



} space_invaders_machine;







#endif
