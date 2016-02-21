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

    createWindow("test", 800, 600, UNDEFINED_WINDOW_POS, UNDEFINED_WINDOW_POS, false, false, true, false);

    glewExperimental = GL_TRUE;
    GLenum glew_err = glewInit();
    if(glew_err != GLEW_OK){
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(glew_err) << std::endl;
        return false;
    }

    Timer::initialize();
    EventHandler::initialize();

    glClearColor(0.f, 0.f, 0.f, 1.f);

    //shader code, remove when components are added
    if(!initShaders()){
        return false;
    }

    if(!initVertDat()){
        return false;
    }

    return true;
}

bool Engine::initShaders(){
    program_ = glCreateProgram();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* vs_source[] = {"#version 330 core\nin vec3 LVertexPos; void main() { gl_Position = vec4( LVertexPos.x, LVertexPos.y, LVertexPos.z, 1 ); }"};
    const GLchar* fs_source[] = {"#version 330 core\nout vec3 color; void main() { color = vec3(1.0, 1.0, 1.0); }"};

    glShaderSource(vs, 1, vs_source, NULL);
    glCompileShader(vs);

    GLint shader_compiled;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &shader_compiled);
    if(shader_compiled != GL_TRUE){
        std::cerr << "Unable to compile vertex shader" << std::endl;
        return false;
    }

    glShaderSource(fs, 1, fs_source, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &shader_compiled);
    if(shader_compiled != GL_TRUE){
        std::cerr << "Unable to compile fragment shader" << std::endl;
        return false;
    }

    glAttachShader(program_, vs);
    glAttachShader(program_, fs);
    glLinkProgram(program_);

    GLint program_linked = GL_TRUE;
    glGetShaderiv(program_, GL_LINK_STATUS, &program_linked);
    if(program_linked != GL_TRUE){
        std::cerr << "Unable to link shaders to program" << std::endl;
        return false;
    }

    vert_location_ = glGetAttribLocation(program_, "LVertexPos");

    return true;
}

bool Engine::initVertDat(){
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    GLfloat vert_dat[] = {-1.0f, -1.0f, 0.0f,
                          1.0f, -1.0f, 0.0f,
                          0.0f, 1.0f, 0.0f};

    GLuint index_dat[] = {0, 1, 2};

    glGenBuffers(1, &vert_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 3, vert_dat, GL_STATIC_DRAW);

    glGenBuffers(1, &idx_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3, index_dat, GL_STATIC_DRAW);

    glEnableVertexAttribArray(vert_location_);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buffer_);
    glVertexAttribPointer(vert_location_, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    glBindVertexArray(0);

    return true;
}

bool Engine::run(){
    EventHandler* event_handler = EventHandler::eventHandler();
    Timer* timer = Timer::timer();

    while(!event_handler->isQuit()){
        if(windows_.size() == 0){
            break;
        }

        timer->frame();
        event_handler->frame();

        std::list<Uint32> windows_to_destroy;

        for(auto& window_pair : windows_){
            Uint32 id = window_pair.first;

            if(event_handler->windowClosed(id)){
                windows_to_destroy.push_back(id);
            }
        }

        for(Uint32 id : windows_to_destroy){
            windows_.erase(id);
        }

//        glClear(GL_COLOR_BUFFER_BIT);
//        glUseProgram(program_);
//        glBindVertexArray(vao_);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_);
//        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
//        glBindVertexArray(0);
//        glUseProgram(0);

        for(auto& window_pair : windows_){
            Window* window = window_pair.second.get();

            window->frame();
        }
    }
}

bool Engine::shutdown(){
    Timer::shutdown();
    EventHandler::shutdown();

    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    SDL_Quit();

    return true;
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
