#ifndef SCENE_H
#define SCENE_H

#include "scenenode.h"
#include <memory>

class Scene
{
friend class Engine;
private:
    std::shared_ptr<SceneNode> root_;

private:
    //forwards entire scene by a frame
    void frame();

public:
    Scene();
    Scene(const Scene& other) = delete;
    ~Scene();

    /**
     * @brief Adds the specified child to the parent. This method fails in the case that /p parent does not exist in the scene, or if
     * /p child is equal to /p parent. In the case that child already exists in the scene, it and all its descendants are removed
     * from the current location and added to the parent.
     * @param parent Parent to add child to
     * @param child Child to add to parent
     * @return true if success, otherwise false
     */
    bool addChild(std::shared_ptr<SceneNode> parent, std::shared_ptr<SceneNode> child);

    /**
     * @brief Adds a child to the passed /p parent. The child is given the specified name /p child_name, or "Nameless" in the case that
     * no name is passed. This method fails if parent does not exist in the scene.
     * @param parent Parent to add child to
     * @param child_name Name of the new child
     * @return a pair with the first element being a boolean indicating success or failure, and the second being the newly created
     * and added child
     */
    std::pair<bool, std::shared_ptr<SceneNode> > addChild(std::shared_ptr<SceneNode> parent, std::string child_name = "Nameless");

    /**
     * @brief Gets the root node of the scene
     * @return shared pointer to the root node
     */
    std::shared_ptr<SceneNode> rootNode();
};

#endif // SCENE_H
