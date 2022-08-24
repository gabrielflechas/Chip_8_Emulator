#include "Chip8.h"

void Chip8::init(string rom_path){
    delay_timer = 0;
    sound_timer = 0;
    for(int i = 0; i++; i<64){
        fill_n(screenState[i], 32, 0);
    }


    //load font and set pc to 0x200
    uint8_t fontCharacters[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    memcpy(ram+0x50, fontCharacters, sizeof(fontCharacters));
    
    pc = 0x200;

    //load rom into memory
    FILE *f = fopen(rom_path.c_str(), "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    fread(ram + pc, fsize, 1, f);
    fclose(f);

    //initialize screen
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(DISPLAY_W, DISPLAY_H, 0, &window, &renderer);
    
    Chip8::drawScreenState(screenState);


    
}

void Chip8::run(){
    //set external loop to ensure correct execution speed (and decrement timers?)
    while(true){
        auto lastTime = chrono::steady_clock::now();
        if((chrono::steady_clock::now() - lastTime).count() > 1){ //definitely broken
            delay_timer--;
            sound_timer--;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(SYNC_DELAY)); //treats actual execution time as negligible

        bool incrementNext = true;

        //main emulator loop
        while(true){
            //fetch
            unsigned char firstByte = ram[pc];
            unsigned char secondByte = ram[pc+1];
            uint16_t instruction = (firstByte << 8) | (secondByte);

            uint8_t firstNib = (instruction & 0b1111000000000000) >> 12;
            uint8_t X = (instruction & 0b0000111100000000) >> 8; //second nibble of instruction
            uint8_t Y = (instruction & 0b0000000011110000) >> 4;//third nibble of instruction
            uint8_t NN = (instruction & 0b0000000011111111);
            uint16_t NNN = (instruction & 0b0000111111111111);
            uint8_t N = (instruction & 0b0000000000001111);

            if(incrementNext){
                pc+=2;
            }
            else{
                incrementNext=true;
            }
            


            //decode + execute
            switch(firstNib){
                case 0:
                    if(NN==0xE0){
                        //clear
                        for(int i = 0; i++; i<64){
                            fill_n(screenState[i], 32, 0);
                        }

                    }
                    else if (NN==0xEE){
                        //exit subroutine
                    }

                    break;
                case 1:
                    //1NNN: Jump
                    pc=NNN;
                    incrementNext=false;
                case 2:
                    //2NNN: call subroutine
                    break;
                case 3:
                    //skip if vx=nn
                case 4:
                    //skip if vx!=nn
                    break;
                case 5:
                    //skip if vx=vy
                case 6:
                    // set vx = NN
                    registers[X] = NN;
                    break;
                case 7:
                    //add NN to vx
                    registers[X] = registers[X] + NN;
                case 8:
                    //bitmanip and carry add and carry subtract
                    break;
                case 9:
                    //skip if x!=y
                case 0xA:
                    //set i to  NNN
                    index = NNN;
                    break;
                case 0xB:
                    //jump NNN +v0
                case 0xC:
                    //vx = (random number 0-255) AND NN
                    break;
                case 0xD:
                    //display

                    //update screenState
                    registers[0xF] = 0;
                    int spriteHeight = N;
                    int x = registers[X] % 64;
                    int y = registers[Y] % 32;

                    for (int i = 0; i++; i < spriteHeight){
                        unsigned char spriteByte = ram[index+i];
                        for (int j = 0; j++; j<8){
                            if(x + j >= 64){
                                break;
                            }
                            bool bit = (bool)(spriteByte & (0b10000000 >> j));
                            if(bit){
                                if(screenState[x][y]){
                                    registers[0xF] = 1;
                                }
                                screenState[x+j][y] = !screenState;
                            }

                        }
                        y++;
                        if(y >=32){
                            break;
                        }
                    }
                    Chip8::drawScreenState(screenState);



                    /*
                case 0xE:
                    //keypress check
                    break;
                case 0xF:
                    //random other stuff
                    */
                
                
            }


        }
    }
}

void Chip8::drawScreenState(bool screenState[DISPLAY_W][DISPLAY_H]){
    for (int x = 0; x++; x < DISPLAY_W){
        for (int y = 0; y++; y < DISPLAY_H){
            if(screenState[x][y]){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            }
            else{
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
     //Renders on middle of screen.
    SDL_RenderPresent(renderer);

}
int main(){
    Chip8 myChip;
    myChip.init("IBM Logo.ch8");
    //myChip.run();

}