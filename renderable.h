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
friend class ResourceManager;
friend class Renderer;

private:
    GLuint vao_name_;
    std::unique_ptr<Material> material_;
    Mesh* mesh_;

private:
    Renderable() = delete;
    Renderable(std::unique_ptr<Material>&& mat, Mesh* mesh);
    Renderable(std::unique_ptr<Material>&& mat, Mesh* mesh, GLuint vao_name);

protected:
    virtual void frameStart();
    virtual void frameEnd();
    virtual void startup();
    virtual void shutdown();

public:
    Renderable(const Renderable& other);
    Renderable& operator = (const Renderable& other);
    ~Renderable();

    /**
     * @brief Gets observer pointer to the Renderable's material
     * @return a pointer to the Renderable's material
     */
    Material* getMaterial();

    /**
     * @brief Gets an observer pointer to the Renderable's mesh
     * @return a pointer to the Renderable's mesh
     */
    Mesh* getMesh();

    /**
     * @brief Gets the vao name of the renderable
     * @return a GLuint containing the name of the Renderable's vao
     */
    GLuint getVAOName();

    virtual std::unique_ptr<Component> clone();
};

#endif // RENDERABLE_H
