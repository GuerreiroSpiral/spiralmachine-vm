#include "SpiralMachine.h"
#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SpiralMachine sp_m = SpiralMachine();
    sp_m.init();
    return 0;
}