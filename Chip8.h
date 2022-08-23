#pragma once

#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <bitset>
#include <stack>
#include <chrono>
#include <thread>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
 
#define SYNC_DELAY 1000/700
#define DISPLAY_H 32
#define DISPLAY_W 64


using namespace std;

class Chip8{
    
    Screen screen;
    unsigned char ram[4*1024];
    uint16_t pc;
    uint16_t index;
    stack<uint16_t> stack;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t registers[16];
}