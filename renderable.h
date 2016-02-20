#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "common.h"
#include "component.h"
#include "material.h"
#include "mesh.h"

#include <memory>
#include <exception>

class RenderableError : public std::exception{
private:
    std::string error_log_;

public:
    RenderableError(std::string log){
        error_log_ = log;
    }

    virtual const char* what() const throw(){
        return error_log_.c_str();
    }
};

class Renderable : public Component
{
private:
    GLuint vao_name_;
    std::unique_ptr<Material> material_;
    Mesh* mesh_;

protected:
    virtual void frameStart();
    virtual void frameEnd();
    virtual void startup();
    virtual void shutdown();

public:
    Renderable() = delete;
    Renderable(std::unique_ptr<Material>&& mat, Mesh* mesh);
    Renderable(std::unique_ptr<Material>&& mat, Mesh* mesh, GLuint vao_name);
    Renderable(const Renderable& other);
    Renderable& operator = (const Renderable& other);
    ~Renderable();

    Material* getMaterial();
    Mesh* getMesh();
    GLuint getVAOName();
};

#endif // RENDERABLE_H
