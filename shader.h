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
friend class ResourceManager;
private:
    std::uint32_t id_;

    GLuint program_;

    std::string lexical_name_;

private:
    //only callable by ShaderManager
    Shader(const std::string& lexical_name, std::uint32_t id, const std::string& vs, const std::string& fs);
    //trhwos ShaderCompileError if compilation or linking fails
    void initializeShader(const std::string& vs, const std::string& fs);
    //helper for initializeShader
    std::string queryShaderErrorMsg(GLuint name);

public:
    Shader() = delete;
    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;
    ~Shader();

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

    /**
     * @brief Gets the lexical name of the shader
     * @return a string containing the lexical name of the shader
     */
    std::string getLexicalName();
};

#endif // SHADER_H
