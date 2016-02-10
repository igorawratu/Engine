#ifndef SCENENODE_H
#define SCENENODE_H

#include <Eigen/Dense>
#include <memory>
#include <string>
#include <list>

class SceneNode
{
friend class Scene;
private:
    std::shared_ptr<SceneNode> parent_;
    std::list<std::shared_ptr<SceneNode> > children_;
    std::string name_;

    Eigen::Quaternion rotation_;
    Eigen::Translation3f translation_;
    Eigen::Scaling scale_;

private:
    void frame(const Eigen::Affine3f& parent_world_transform_, );

public:
    SceneNode();
    SceneNode(std::string name, std::shared_ptr<SceneNode> parent);
    SceneNode(const SceneNode& other);
    ~SceneNode();

    std::string name();

    Eigen::Affine3f parentWorldTransform();

    Eigen::Quaternion rotation();
    Eigen::Translation3f translation();
    Eigen::Scaling scale();

    void rotation(const Quaternion& rot);
    void rotateBy(const Eigen::Quaternion& rot);
    void translation(const Eigen::Translation3f& trans);
    void scale(const Eigen::Scaling& scl);

    Eigen::Quaternion worldRotation();
    Eigen::Translation3f worldTranslation();
    Eigen::Scaling worldScale();


};

#endif // SCENENODE_H
