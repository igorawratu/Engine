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
    engine->run();
    engine->shutdown();

    return 0;
}

