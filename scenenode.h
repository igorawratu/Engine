#ifndef SCENENODE_H
#define SCENENODE_H

#include <Eigen/Geometry>
#include <memory>
#include <string>
#include <list>

class SceneNode
{
friend class Scene;
private:
    SceneNode* parent_;
    std::list<std::shared_ptr<SceneNode> > children_;
    std::string name_;

    Eigen::Quaternion<float> rotation_;
    Eigen::Vector3f translation_;
    Eigen::Vector3f scale_;

private:
    void frame(const Eigen::Affine3f& parent_world_transform_);

public:
    SceneNode();
    SceneNode(std::string name, SceneNode* parent);
    SceneNode(const SceneNode& other) = default;
    ~SceneNode();

    /**
     * @brief Gets name of the SceneNode
     * @return a string containing the name
     */
    std::string name();

    /**
     * @brief Calculates world transform of the SceneNode
     * @return world transform as an Eigen::Affine3f
     */
    Eigen::Affine3f worldTransform();

    /**
     * @brief Gets local rotation of the SceneNode
     * @return an Eigen::Quaternion depicting the local rotation
     */
    Eigen::Quaternion<float> rotation();

    /**
     * @brief Gets local translation of the SceneNode
     * @return an Eigen::Vector3f depicting the local translation
     */
    Eigen::Vector3f translation();

    /**
     * @brief Gets scale of the SceneNode
     * @return an Eigen::Vector3f depicting the scale
     */
    Eigen::Vector3f scale();

    /**
     * @brief Sets rotation of SceneNode to /p rot
     * @param rot The rotation to be set
     */
    void rotation(const Eigen::Quaternion<float>& rot);

    /**
     * @brief Rotates the SceneNode by /p rot
     * @param rot The rotation to add
     */
    void rotateBy(const Eigen::Quaternion<float>& rot);

    /**
     * @brief Sets translation of SceneNode to /p trans
     * @param trans The translation to be set
     */
    void translation(const Eigen::Vector3f& trans);

    /**
     * @brief Sets the scale of the SceneNode to /p scl
     * @param scl The scale to be set
     */
    void scale(const Eigen::Vector3f& scl);

    /**
     * @brief Gets world rotation of the SceneNode
     * @return the world rotation of the SceneNode as an Eigen::Quaternion
     */
    Eigen::Quaternion<float> worldRotation();

    /**
     * @brief Gets the world translation of the SceneNode
     * @return the world translation of the SceneNode as an Eigen::Vector3f
     */
    Eigen::Vector3f worldTranslation();

    /**
     * @brief Appends a child to the SceneNode
     * @param child the child to append
     */
    void addChild(const std::shared_ptr<SceneNode>& child);

    /**
     * @brief Finds the first occurence of a child with the specified /p name. All descendents are searched, using depth first
     * @param name Name of the child to find
     * @return a shared pointer to the first descedant with the given name
     */
    std::shared_ptr<SceneNode> findChild(const std::string& name);

    /**
     * @brief Finds all descendents with the given /p name. Search is performed depth first, with the children being in that order.
     * @param name Name of the children to find
     * @return a list of shared pointers to all the children with the given name
     */
    std::list<std::shared_ptr<SceneNode> > findChildren(const std::string& name);
};

#endif // SCENENODE_H
