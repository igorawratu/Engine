#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include <memory>

#include "inputhandler.h"
#include "timer.h"
#include "window.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

using namespace std;

int main(int argc, char *argv[])
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Error: SDL unable to initialize", SDL_GetError() );
        return 0;
    }

    Window w("Hello", 800, 600, CENTER_WINDOW_POS, CENTER_WINDOW_POS, true, true, true, false);

    SDL_Delay(5000);

    SDL_Quit();
    return 0;
}

