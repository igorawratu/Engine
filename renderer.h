#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"

#include <unordered_map>
#include <list>
#include <memory>
#include <vector>

class Renderable;
class Camera;

class Renderer
{
friend std::unique_ptr<Renderer>::deleter_type;
private:
    std::unordered_map<GLuint, std::list<Renderable*> > renderables_;
    std::vector<Camera*> cameras_;

    static std::unique_ptr<Renderer> renderer_;

private:
    Renderer();
    ~Renderer();

    static bool initialize();
    static bool shutdown();

public:
    Renderer(const Renderer& other) = delete;
    Renderer& operator = (const Renderer& other) = delete;

    static Renderer* renderer();

    /**
     * @brief Advances renderer by a frame
     */
    void frame();

    /**
     * @brief Adds renderable to be rendered for the current frame
     * @param renderable Renderable to be rendered for the current frame
     */
    void addRenderable(Renderable* renderable);

    /**
     * @brief Adds camera to be used for the current frame. Multiple cameras results in multiple viewports.
     * @param camera Camera to be used for the current frame
     */
    void addCamera(Camera* camera);
};

#endif // RENDERER_H
