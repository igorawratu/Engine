#ifndef ENGINE_H
#define ENGINE_H

#include "common.h"

#include <memory>
#include <unordered_map>

#include "eventhandler.h"
#include "timer.h"
#include "window.h"
#include "resourcemanager.h"

class Engine
{
friend std::unique_ptr<Engine>::deleter_type;

private:
    static std::unique_ptr<Engine> engine_;

    bool initialized_;

    std::unique_ptr<Window> window_;

private:
    Engine();
    Engine(const Engine& other) = delete;
    ~Engine();

    Window* createWindow(std::string title, int width, int height, int x_pos, int y_pos, bool maximized,
                         bool fullscreen, bool resizable, bool focus);

public:
    static Engine* engine();

    bool startup(std::string title = "Mazz's Long Wang", int width = 0, int height = 0, int x_pos = UNDEFINED_WINDOW_POS, int y_pos = UNDEFINED_WINDOW_POS, bool maximized = true,
                 bool fullscreen = true, bool resizable = true, bool focus = true);
    bool run();
    bool shutdown();

    Window* window();
};

#endif // ENGINE_H
