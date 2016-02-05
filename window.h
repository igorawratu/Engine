#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <SDL_opengl.h>

#include <string>

const int CENTER_WINDOW_POS = -1;
const int UNDEFINED_WINDOW_POS = -2;

class Window
{
private:
    SDL_Window* window_;
    SDL_GLContext context_;

public:
    Window(std::string title = "Engine", int width = 0, int height = 0, int x_pos = UNDEFINED_WINDOW_POS, int y_pos = UNDEFINED_WINDOW_POS, bool maximized = true,
           bool fullscreen = true, bool resizable = true, bool focus = true);
    //do not allow copying windows
    Window(const Window& other) = delete;
    ~Window();

    void makeCurrent();

    Uint32 getID();

    std::pair<int, int> getResolution();

    std::pair<int, int> getPosition();

    bool isVisible();

    bool isMinimized();

    bool inputGrabbed();

    bool isInputFocused();

    bool isMouseOver();

    bool isResizable();

    bool isBorderless();

    bool isFullScreen();

    void resizeWindow(int x, int y);

    void frame();
};

#endif // WINDOW_H
