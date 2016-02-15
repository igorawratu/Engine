#include "shader.h"

Shader::Shader(std::uint32_t id, const std::string& vs, const std::string& fs) : id_(id), program_(0){
    initializeShader(vs, fs);
}

void Shader::initializeShader(const std::string& vs, const std::string& fs){
    program_ = glCreateProgram();

    GLuint vs_name = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs_name = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vs_source = vs.c_str();
    const char* fs_source = fs.c_str();

    glShaderSource(vs_name, 1, &vs_source, NULL);
    glCompileShader(vs_name);

    GLint shader_compiled;
    glGetShaderiv(vs_name, GL_COMPILE_STATUS, &shader_compiled);
    if(shader_compiled != GL_TRUE){
        std::string err = queryShaderErrorMsg(vs_name);
        throw ShaderCompileError(err);
    }

    glShaderSource(fs_name, 1, &fs_source, NULL);
    glCompileShader(fs_name);

    glGetShaderiv(fs_name, GL_COMPILE_STATUS, &shader_compiled);
    if(shader_compiled != GL_TRUE){
        std::string err = queryShaderErrorMsg(fs_name);
        throw ShaderCompileError(err);
    }

    glAttachShader(program_, vs_name);
    glAttachShader(program_, fs_name);
    glLinkProgram(program_);

    GLint program_linked = GL_TRUE;
    glGetShaderiv(program_, GL_LINK_STATUS, &program_linked);
    if(program_linked != GL_TRUE){
        std::string err = queryShaderErrorMsg(program_);
        throw ShaderCompileError(err);
    }
}

std::uint32_t Shader::getID(){
    return id_;
}

GLuint Shader::getProgram(){
    return program_;
}

std::string Shader::queryShaderErrorMsg(GLuint name){
    GLint log_length;
    glGetShaderiv(name, GL_INFO_LOG_LENGTH , &log_length);

    GLchar info_log[log_length + 1];
    glGetShaderInfoLog(name, log_length, NULL, info_log);

    std::string msg(info_log);

    return msg;
}
