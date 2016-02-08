#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include <memory>

#include "engine.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

using namespace std;

int main(int argc, char *argv[])
{
    Engine* engine = Engine::engine();

    engine->startup();
    engine->createWindow("Test", 800, 600, UNDEFINED_WINDOW_POS, UNDEFINED_WINDOW_POS, false, false, true, false);
    engine->run();
    engine->shutdown();

    return 0;
}

