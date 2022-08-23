#include "Chip8.h"

void init(string rom_path){
    this.delay_timer = 0;
    this.sound_timer = 0;

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
    
    this.pc = 0x200;

    //load rom into memory
    FILE *f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    fread(this.ram + pc, fsize, 1, f);
    fclose(f);
}

void run(){
    //set external loop to ensure correct execution speed (and decrement timers?)
    while(true){
        auto lastTime = chrono::steady_clock::now();
        if(chrono::steady_clock::now() - lastTime > 1){
            this.delay_timer--;
            this.sound_timer--;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(SYNC_DELAY)); //treats actual execution time as negligible

        //main emulator loop
        while(true){
            //fetch
            unsigned char firstByte = ram[pc];
            unsigned char secondByte = ram[pc+1];
            uint16_t instruction = (firstByte << 8) | (secondByte);

            uint8_t firstNib = (instruction & 0b1111000000000000) >> 12;
            uint8_t X = (instruction & 0b0000111100000000) >> 8;
            uint8_t Y = (instruction & 0b0000000011110000) >> 4;
            uint8_t NN = (instruction & 0b0000000011111111) 
            uint16_t NNN = (instruction & 0b0000111111111111)

            
            pc+=2;


            //decode + execute


        }
    }

    


}