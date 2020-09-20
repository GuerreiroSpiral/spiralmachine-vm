#include "SpiralScreen.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h> 

SpiralScreen::SpiralScreen(){
    window = SDL_CreateWindow("SpiralMachine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH + 64, SCREEN_HEIGHT + 64, SDL_WINDOW_SHOWN);
    if (!window){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Window Error", "Couldn't initialize the SDL Window", NULL);
        exit(-1);
    }
    surface = SDL_GetWindowSurface(window);
    if (!surface){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Surface Error", "Couldn't initialize the SDL Surface", NULL);
        exit(-1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Renderer", "Couldn't initialize the SDL Renderer", NULL);
        exit(-1);
    }
    SDL_RenderSetScale(renderer, 3, 3);
}

SpiralScreen::~SpiralScreen(){
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SpiralScreen::blit_screen(uint8_t color, uint16_t VRAM_LOCATION){
    short red = (color >> 6) * COLOR_MUL_OFFSET;
    short green = ((color >> 4) & 0x3)  * COLOR_MUL_OFFSET;
    short blue = ((color >> 2) & 0x3) * COLOR_MUL_OFFSET;

    short x = (VRAM_LOCATION - 4) % SCREEN_WIDTH;
    short y = (VRAM_LOCATION - 4) / SCREEN_HEIGHT;

    SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
    SDL_RenderDrawPoint(renderer, x, y);
}

void SpiralScreen::render_screen(uint8_t memory[]){
    for (int i = VRAM_M_START; i <= VRAM_M_END; i++){
        blit_screen(memory[i], i);
    }
    SDL_RenderPresent(renderer);
}