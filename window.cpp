#include "window.h"
#include <iostream>

Window::Window(std::string title, int width, int height, int x_pos, int y_pos, bool maximized,
       bool fullscreen, bool resizable, bool focus){
    assert(width >= 0 && height >= 0);

    Uint32 flags = SDL_WINDOW_OPENGL;

    if(width == 0 || height == 0){
        SDL_DisplayMode curr_dmode;
        int success = SDL_GetCurrentDisplayMode(0, &curr_dmode);
        if(success < 0){
            std::cerr << "Error: unable to acquire dimensions of screen, please pass in actual window width and height" << std::endl;
        }

        if(width == 0){
            width = curr_dmode.w;
        }

        if(height == 0){
            height = curr_dmode.h;
        }
    }

    if(x_pos == CENTER_WINDOW_POS){
        x_pos = SDL_WINDOWPOS_CENTERED;
    }
    else if(x_pos == UNDEFINED_WINDOW_POS){
        x_pos = SDL_WINDOWPOS_UNDEFINED;
    }

    if(y_pos == CENTER_WINDOW_POS){
        y_pos = SDL_WINDOWPOS_CENTERED;
    }
    else if(y_pos == UNDEFINED_WINDOW_POS){
        y_pos = SDL_WINDOWPOS_CENTERED;
    }

    if(maximized){
        flags |= SDL_WINDOW_MAXIMIZED;
    }

    if(fullscreen){
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    if(resizable){
        flags |= SDL_WINDOW_RESIZABLE;
    }

    if(focus){
        flags |= SDL_WINDOW_INPUT_GRABBED;
    }

    window_ = SDL_CreateWindow(title.c_str(), x_pos, y_pos, width, height, flags);
    context_ = SDL_GL_CreateContext(window_);
}

Window::~Window(){
    SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(window_);
}

void Window::makeCurrent(){
    int success = SDL_GL_MakeCurrent(window_, context_);

    if(success < 0){
        std::cerr << "Error: unable to make window current" << std::endl;
    }
}

Uint32 Window::getID(){
    Uint32 id = SDL_GetWindowID(window_);

    return id;
}

std::pair<int, int> Window::getResolution(){
    std::pair<int, int> resolution;

    SDL_GetWindowSize(window_, &resolution.first, &resolution.second);

    return resolution;
}

std::pair<int, int> Window::getPosition(){
    std::pair<int, int> position;

    SDL_GetWindowPosition(window_, &position.first, &position.second);

    return position;
}

bool Window::isVisible(){
    Uint32 flags = SDL_GetWindowFlags(window_);

    return flags & SDL_WINDOW_SHOWN;
}

bool Window::isMinimized(){
    Uint32 flags = SDL_GetWindowFlags(window_);

    return flags & SDL_WINDOW_MINIMIZED;
}

bool Window::isMaximized(){
    Uint32 flags = SDL_GetWindowFlags(window_);

    return flags & SDL_WINDOW_MAXIMIZED;
}

bool Window::inputGrabbed(){
    Uint32 flags = SDL_GetWindowFlags(window_);

    return flags & SDL_WINDOW_INPUT_GRABBED;
}

bool Window::isInputFocused(){
    Uint32 flags = SDL_GetWindowFlags(window_);

    return flags & SDL_WINDOW_INPUT_FOCUS;
}

bool Window::isMouseOver(){
    Uint32 flags = SDL_GetWindowFlags(window_);

    return flags & SDL_WINDOW_MOUSE_CAPTURE;
}

bool Window::isResizable(){
    Uint32 flags = SDL_GetWindowFlags(window_);

    return flags & SDL_WINDOW_RESIZABLE;
}

bool Window::isBorderless(){
    Uint32 flags = SDL_GetWindowFlags(window_);

    return flags & SDL_WINDOW_BORDERLESS;
}

bool Window::isFullScreen(){
    Uint32 flags = SDL_GetWindowFlags(window_);

    return flags & SDL_WINDOW_FULLSCREEN;
}

void Window::resizeWindow(int width, int height){
    assert(width > 0 && height > 0);

    bool fullscreen = isFullScreen();
    if(fullscreen){
        SDL_SetWindowFullscreen(window_, 0);
    }

    SDL_SetWindowSize(window_, width, height);

    if(fullscreen){
        SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
    }
}

void Window::frame(){
    SDL_GL_SwapWindow(window_);
}
