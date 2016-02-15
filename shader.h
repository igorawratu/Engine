#ifndef SHADER_H
#define SHADER_H

#include "common.h"
#include <string>
#include <exception>

class ShaderCompileError : public std::exception{
private:
    std::string error_log_;

public:
    ShaderCompileError(std::string log){
        error_log_ = log;
    }

    virtual const char* what() const throw(){
        return error_log_.c_str();
    }
};

class Shader
{
friend class ShaderManager;
private:
    std::uint32_t id_;

    GLuint program_;

private:
    //only callable by ShaderManager
    Shader(std::uint32_t id, const std::string& vs, const std::string& fs);
    //trhwos ShaderCompileError if compilation or linking fails
    void initializeShader(const std::string& vs, const std::string& fs);
    //helper for initializeShader
    std::string queryShaderErrorMsg(GLuint name);

public:
    Shader() = delete;
    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;

    /**
     * @brief Gets ID of shader. This can be used to query the ShaderManager for the shader later.
     * @return ID of shader
     */
    std::uint32_t getID();

    /**
     * @brief Gets program name of shader
     * @return
     */
    GLuint getProgram();
};

#endif // SHADER_H
