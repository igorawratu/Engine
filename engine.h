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

    std::unordered_map<Uint32, std::unique_ptr<Window> > windows_;

    GLuint program_;
    GLint vert_location_;
    GLuint vert_buffer_;
    GLuint idx_buffer_;
    GLuint vao_;

private:
    Engine();
    Engine(const Engine& other) = delete;
    ~Engine();

public:
    static Engine* engine();

    bool startup();
    bool run();
    bool shutdown();

    Window* windowByID(Uint32 windowID);
    Window* createWindow(std::string title = "Engine", int width = 0, int height = 0, int x_pos = UNDEFINED_WINDOW_POS, int y_pos = UNDEFINED_WINDOW_POS, bool maximized = true,
                         bool fullscreen = true, bool resizable = true, bool focus = true);
};

#endif // ENGINE_H
