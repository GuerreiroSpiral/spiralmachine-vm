#ifndef SPIRAL_SCREEN_H
#define SPIRAL_SCREEN_H

#include <stdint.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 32
#define SCREEN_HEIGHT 32
#define COLOR_MUL_OFFSET 64
#define VRAM_M_START 0x0004
#define VRAM_M_END 0x0404
#define VRAM_SIZE 1024


class SpiralScreen{
    SDL_Window *window = NULL;
    SDL_Surface *surface = NULL;
    SDL_Renderer *renderer = NULL;
    public:
        SpiralScreen();
        ~SpiralScreen();
        void render_screen(uint8_t memory[]);
         void blit_screen (uint8_t color, uint16_t VRAM_LOCATION);
};

#endif // SPIRAL_SCREEN_H