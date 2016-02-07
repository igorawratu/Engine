#ifndef ENGINE_H
#define ENGINE_H

#include <SDL.h>
#include <SDL_opengl.h>

#include <memory>

#include "eventhandler.h"
#include "timer.h"
#include "window.h"

enum WindowCloseAction{WINDOW_DESTROY, WINDOW_MINIMIZE, NONE};

class Engine
{
private:
    static std::unique_ptr<Engine> engine_ = nullptr;

    std::unique_ptr<Timer> timer_;
    std::unique_ptr<EventHandler> event_handler_;

private:
    Engine();
    Engine(const Engine& other) = delete;
    ~Engine();

public:
    static Engine* engine();

    bool startup();
    bool run();
    bool shutdown();

    Timer* timer();

    EventHandler* eventHandler();

    Window* windowByID(Uint32 windowID);
    Window* createWindow(std::string title = "Engine", int width = 0, int height = 0, int x_pos = UNDEFINED_WINDOW_POS, int y_pos = UNDEFINED_WINDOW_POS, bool maximized = true,
                         bool fullscreen = true, bool resizable = true, bool focus = true, WindowCloseAction action = WINDOW_DESTROY);
};

#endif // ENGINE_H
