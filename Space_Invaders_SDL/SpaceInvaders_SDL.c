#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL.h>
#include <time.h>
#include "8080_core_SDL.h"
#include "8080_core_SDL.c"
#include "SpaceInvaders_SDL.h"


#define SCREEN_SCALE_AMOUNT 3


void readROM(char *file_name, uint8_t *Memory) {
  FILE *fp;
  fp = fopen(file_name, "rb");
  fseek(fp, 0, SEEK_END);
  int size_of_ROM = ftell(fp);
  rewind(fp);
  uint8_t *ROM_code_buffer_temp =  malloc(size_of_ROM); //buffer array which contains binary code
  fread(ROM_code_buffer_temp, size_of_ROM, 1, fp);
  fclose(fp);   // close file
  for (int i = 0; i < size_of_ROM; i++) {
  Memory[i] = ROM_code_buffer_temp[i];
  }
  //memcpy(Memory,ROM_code_buffer_temp,size_of_ROM);
  free(ROM_code_buffer_temp);
}

void render(SDL_Renderer *renderer, uint8_t *Memory) {
  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
  //SDL_RenderFillRect(renderer, NULL);
  SDL_RenderClear(renderer);
  int completed_bytes = 0;
  int pos_x = 0;
  int pos_y = 256;




  for (uint16_t j = 0x2400; j < 0x3FFF; j++) {
    for (int k = 0; k < 8; k++) {
      if ((Memory[j] >> k) & 0x01) {
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderDrawPoint(renderer, pos_x, pos_y);

      }
      pos_y--;
    }
    completed_bytes++;
    if (completed_bytes == 32) {

      completed_bytes = 0;
      pos_y = 256;
      pos_x++;
    }

    if (pos_x == 224) {
      pos_x = 0;
      pos_y = 256;
    }
  }

  SDL_RenderPresent(renderer);
}

void renderBIG(SDL_Renderer *renderer, uint8_t *Memory) {
  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
  //SDL_RenderFillRect(renderer, NULL);
  SDL_RenderClear(renderer);
  int completed_bytes = 0;
  int pos_x = 0;
  int pos_y = 256;
  SDL_Rect pixel;
  pixel.w = SCREEN_SCALE_AMOUNT, pixel.h = SCREEN_SCALE_AMOUNT;
  SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );


  for (uint16_t j = 0x2400; j < 0x3FFF; j++) {
    for (int k = 0; k < 8; k++) {
      if ((Memory[j] >> k) & 0x01) {
        
		pixel.y = pos_y * SCREEN_SCALE_AMOUNT;
		pixel.x = pos_x * SCREEN_SCALE_AMOUNT;
		SDL_RenderFillRect(renderer, &pixel);
      }
      pos_y--;
    }
    completed_bytes++;
    if (completed_bytes == 32) {

      completed_bytes = 0;
      pos_y = 256;
      pos_x++;
    }

    if (pos_x == 224) {
      pos_x = 0;
      pos_y = 256;
    }
  }

  SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
  space_invaders_machine spaceinvaders; //create machine
  //memset(spaceinvaders.Memory,0x00,0xFFFF); //set all elements of memory to zero
  for (int p = 0; p < 0xFFFF; p++) {
	spaceinvaders.Memory[p] = 0x00;

  }
  readROM(argv[1], spaceinvaders.Memory); //read ROM into memory
  init_cpu(&spaceinvaders.CPU); //init cpu
  //setting in registers for settings
  spaceinvaders.CPU.IN[0] = 0x0F;
  spaceinvaders.CPU.IN[1] = 0x08;
  spaceinvaders.CPU.IN[2] = 0x80;

  /***********************START OF SDL SHIT*************************************/
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = NULL;
  window = SDL_CreateWindow( "Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 224*SCREEN_SCALE_AMOUNT, 256*SCREEN_SCALE_AMOUNT, SDL_WINDOW_SHOWN );
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Event e;

  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
  SDL_RenderFillRect(renderer, NULL);
  SDL_RenderPresent( renderer );
	

  const uint8_t *state = SDL_GetKeyboardState(NULL);

	uint16_t startPC = 0x14E0, endPC = 0x1597; //for debugging
	bool debugging = false;
	int framesPerSecond = 0;
	time_t lastTime= time(NULL);


  //MAIN LOOP
  while (spaceinvaders.CPU.running == 0x01){
    //update_IN()
    while (SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT) {
        spaceinvaders.CPU.running = 0x00;
      }
    }
	
	if (state[SDL_SCANCODE_S]) {
		spaceinvaders.CPU.IN[1] |= 0x04;
	} else {
		spaceinvaders.CPU.IN[1] &= 0xFB;
	}
	
	if (state[SDL_SCANCODE_C]) {
		spaceinvaders.CPU.IN[1] |= 0x01;
	} else {
		spaceinvaders.CPU.IN[1] &= 0xFE;
	}
	
	if (state[SDL_SCANCODE_SPACE]) {
		spaceinvaders.CPU.IN[1] |= 0x10;
	} else {
		spaceinvaders.CPU.IN[1] &= 0xEF;
	}
	
	if (state[SDL_SCANCODE_LEFT]) {
		spaceinvaders.CPU.IN[1] |= 0x20;
	} else {
		spaceinvaders.CPU.IN[1] &= 0xDF;
	}
	
	if (state[SDL_SCANCODE_RIGHT]) {
		spaceinvaders.CPU.IN[1] |= 0x40;
	} else {
		spaceinvaders.CPU.IN[1] &= 0xBF;
	}
	
    
    for (int i = 0; i < 14285;){ //14285 28527
		i += e8080_perform_instruction(&spaceinvaders.CPU, spaceinvaders.Memory, 22222);
		if ((spaceinvaders.CPU.PC > startPC && spaceinvaders.CPU.PC < endPC) && debugging) {
			printf("%04X	A: %02X  B: %02X  C: %02X  D: %02X  E: %02X  H: %02X  L: %02X",spaceinvaders.CPU.PC, spaceinvaders.CPU.reg_A, spaceinvaders.CPU.reg_B, spaceinvaders.CPU.reg_C, spaceinvaders.CPU.reg_D, spaceinvaders.CPU.reg_E, spaceinvaders.CPU.reg_H, spaceinvaders.CPU.reg_L);
			getchar();
		}


    }
    if (spaceinvaders.CPU.interupts_enabled) {
      e8080_interrupt_cpu(&spaceinvaders.CPU, spaceinvaders.Memory, 1);
		if ((spaceinvaders.CPU.PC > startPC && spaceinvaders.CPU.PC < endPC) && debugging) {
			printf("%04X	A: %02X  B: %02X  C: %02X  D: %02X  E: %02X  H: %02X  L: %02X",spaceinvaders.CPU.PC, spaceinvaders.CPU.reg_A, spaceinvaders.CPU.reg_B, spaceinvaders.CPU.reg_C, spaceinvaders.CPU.reg_D, spaceinvaders.CPU.reg_E, spaceinvaders.CPU.reg_H, spaceinvaders.CPU.reg_L);
			getchar();
		}
    }

    for (int j = 0; j < 19047 ;){//4839  19047
      j += e8080_perform_instruction(&spaceinvaders.CPU, spaceinvaders.Memory, 11111);
	  
		if ((spaceinvaders.CPU.PC > startPC && spaceinvaders.CPU.PC < endPC) && debugging) {
			printf("%04X	A: %02X  B: %02X  C: %02X  D: %02X  E: %02X  H: %02X  L: %02X",spaceinvaders.CPU.PC, spaceinvaders.CPU.reg_A, spaceinvaders.CPU.reg_B, spaceinvaders.CPU.reg_C, spaceinvaders.CPU.reg_D, spaceinvaders.CPU.reg_E, spaceinvaders.CPU.reg_H, spaceinvaders.CPU.reg_L);
			getchar();
		}
    }
    if (spaceinvaders.CPU.interupts_enabled) {
		e8080_interrupt_cpu(&spaceinvaders.CPU, spaceinvaders.Memory, 2);
		
		if ((spaceinvaders.CPU.PC > startPC && spaceinvaders.CPU.PC < endPC) && debugging) {
			printf("%04X	A: %02X  B: %02X  C: %02X  D: %02X  E: %02X  H: %02X  L: %02X",spaceinvaders.CPU.PC, spaceinvaders.CPU.reg_A, spaceinvaders.CPU.reg_B, spaceinvaders.CPU.reg_C, spaceinvaders.CPU.reg_D, spaceinvaders.CPU.reg_E, spaceinvaders.CPU.reg_H, spaceinvaders.CPU.reg_L);
			getchar();
		}
    }
    
    renderBIG(renderer, spaceinvaders.Memory);
	
	if (time(NULL) == lastTime) {
		framesPerSecond++;
	} else {
		printf("%d\n", framesPerSecond);
		lastTime = time(NULL);
		framesPerSecond = 0;
	}
	
    SDL_Delay(7);


  }


  SDL_DestroyRenderer( renderer );
  SDL_DestroyWindow( window ); window = NULL;
  //Quit SDL subsystems
  SDL_Quit();
  return 0;
}
