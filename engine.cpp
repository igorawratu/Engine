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

Engine::Engine() : initialized_(false){
}

Engine::~Engine(){

}

Engine* Engine::engine(){
    if(Engine::engine_ == nullptr){
        Engine::engine_ = std::unique_ptr<Engine>(new Engine());
    }

    return Engine::engine_.get();
}

bool Engine::startup(std::string title, int width, int height, int x_pos, int y_pos, bool maximized,
                     bool fullscreen, bool resizable, bool focus){
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

    Timer::initialize();
    EventHandler::initialize();
    ResourceManager::initialize();

    createWindow(title, width, height, x_pos, y_pos, maximized, fullscreen, resizable, focus);

    glewExperimental = GL_TRUE;
    GLenum glew_err = glewInit();
    if(glew_err != GLEW_OK){
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(glew_err) << std::endl;
        return false;
    }

    glClearColor(0.f, 0.f, 0.f, 1.f);

    return true;
}

bool Engine::run(){
    EventHandler* event_handler = EventHandler::eventHandler();
    Timer* timer = Timer::timer();

    while(!event_handler->isQuit()){
        timer->frame();
        event_handler->frame();

        window_->frame();
    }
}

bool Engine::shutdown(){
    Timer::shutdown();
    EventHandler::shutdown();
    ResourceManager::shutdown();

    window_ = nullptr;

    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    SDL_Quit();

    return true;
}

Window* Engine::window(){
    return window_.get();
}

Window* Engine::createWindow(std::string title, int width, int height, int x_pos, int y_pos, bool maximized, bool fullscreen,
                     bool resizable, bool focus){
    window_ = std::move(std::unique_ptr<Window>(new Window(title, width, height, x_pos, y_pos, maximized, fullscreen, resizable, focus)));
    return window_.get();
}
