#include "engine.h"
#include <iostream>
#include <string>
#include <list>

bool checkSDLErrors(int code, std::string message){
    if(code < 0){
        std::cerr << "Error: " << message << std::endl;

        return false;
    }

    return true;
}

std::unique_ptr<Engine> Engine::engine_ = nullptr;

Engine::Engine()
{
    initialized_ = false;
}

Engine::~Engine(){

}

Engine* Engine::engine(){
    if(Engine::engine_ == nullptr){
        Engine::engine_ = std::unique_ptr<Engine>(new Engine());
    }

    return Engine::engine_.get();
}

bool Engine::startup(){
    if(!checkSDLErrors(SDL_Init(SDL_INIT_VIDEO), "SDL unable to initialize")){
        return false;
    }

    if(!checkSDLErrors(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3), "OpenGL major version 3 not supported")){
        return false;
    }

    if(!checkSDLErrors(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3), "OpenGL minor version 3 not supported")){
        return false;
    }

    if(!checkSDLErrors(SDL_Init(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)), "Unable to set OpenGL core profile")){
        return false;
    }

    timer_ = std::unique_ptr<Timer>(new Timer());
    event_handler_ = std::unique_ptr<EventHandler>(new EventHandler());

    return true;
}

bool Engine::run(){
    while(!event_handler_->isQuit()){
        timer_->frame();
        event_handler_->frame();

        std::list<Uint32> windows_to_destroy;

        for(auto& window_pair : windows_){
            Uint32 id = window_pair.first;

            if(event_handler_->windowClosed(id)){
                windows_to_destroy.push_back(id);
            }
        }

        for(Uint32 id : windows_to_destroy){
            windows_.erase(id);
        }

        for(auto& window_pair : windows_){
            Window* window = window_pair.second.get();

            window->frame();
        }
    }
}

bool Engine::shutdown(){
    timer_ = nullptr;
    event_handler_ = nullptr;

    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    SDL_Quit();

    return true;
}

Timer* Engine::timer(){
    return timer_.get();
}

EventHandler* Engine::eventHandler(){
    return event_handler_.get();
}

Window* Engine::windowByID(Uint32 windowID){
    if(windows_.find(windowID) == windows_.end()){
        return nullptr;
    }

    return windows_[windowID].get();
}

Window* Engine::createWindow(std::string title, int width, int height, int x_pos, int y_pos, bool maximized, bool fullscreen,
                     bool resizable, bool focus){
    std::unique_ptr<Window> window(new Window(title, width, height, x_pos, y_pos, maximized, fullscreen, resizable, focus));
    Uint32 id = window->getID();

    windows_[id] = std::move(window);
}
