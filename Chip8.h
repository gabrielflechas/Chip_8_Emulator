#pragma once

#include <iostream>
#include <algorithm>
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
#include <SDL2/SDL.h>
 
#define SYNC_DELAY 1000/700
#define DISPLAY_W 64
#define DISPLAY_H 32


using namespace std;

class Chip8{
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool screenState[DISPLAY_W][DISPLAY_H];
    unsigned char ram[4*1024];
    uint16_t pc;
    uint16_t index;
    stack<uint16_t> myStack;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t registers[16];
    void drawScreenState(bool screenState[DISPLAY_W][DISPLAY_H]);
    public:
        void init(string path);
        void run();
};