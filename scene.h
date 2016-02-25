#ifndef SCENE_H
#define SCENE_H

#include "scenenode.h"
#include <memory>

class Scene
{
friend class Window;
private:
    std::unique_ptr<SceneNode> root_;

private:
    //forwards entire scene by a frame
    void frame();

public:
    Scene();
    Scene(const Scene& other) = delete;
    ~Scene();

    /**
     * @brief Gets the root node of the scene
     * @return shared pointer to the root node
     */
    SceneNode* rootNode();
};

#endif // SCENE_H
