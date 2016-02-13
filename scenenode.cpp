#include "scenenode.h"

SceneNode::SceneNode() : SceneNode("Nameless"){
}

SceneNode::SceneNode(std::string name) : parent_(nullptr), name_(name), rotation_(Eigen::Quaternion<float>::Identity()),
                                         translation_(0.f, 0.f, 0.f), scale_(1.f, 1.f, 1.f){
}

SceneNode::~SceneNode(){
}

std::string SceneNode::name(){
    return name_;
}

Eigen::Affine3f SceneNode::worldTransform(){
    Eigen::Affine3f local_transform = Eigen::Translation3f(translation_) * rotation_ * Eigen::Scaling(scale_);
    if(parent_ != nullptr){
        return parent_->worldTransform() * local_transform;
    }
    else return local_transform;
}

Eigen::Quaternion<float> SceneNode::rotation(){
    return rotation_;
}

Eigen::Vector3f SceneNode::translation(){
    return translation_;
}

Eigen::Vector3f SceneNode::scale(){
    return scale_;
}

void SceneNode::rotation(const Eigen::Quaternion<float>& rot){
    rotation_ = rot;
}

void SceneNode::rotateBy(const Eigen::Quaternion<float>& rot){
    rotation_ = rot * rotation_;
}

void SceneNode::translation(const Eigen::Vector3f& trans){
    translation_ = trans;
}

void SceneNode::scale(const Eigen::Vector3f& scl){
    scale_ = scl;
}

Eigen::Quaternion<float> SceneNode::worldRotation(){
    if(parent_ == nullptr){
        return rotation_;
    }
    else return parent_->worldRotation() * rotation_;
}

Eigen::Vector3f SceneNode::worldTranslation(){
    if(parent_ == nullptr){
        return translation_;
    }
    else return parent_->worldTransform() * translation_;
}

void SceneNode::addChild(const std::shared_ptr<SceneNode>& child){
    child->parent_ = this;
    children_.push_back(child);
}

std::shared_ptr<SceneNode> SceneNode::findChild(const std::string& name){
    for(auto& child : children_){
        if(child->name() == name){
            return child;
        }

        auto grand_child = child->findChild(name);
        if(grand_child != nullptr){
            return grand_child;
        }
    }

    return nullptr;
}

bool SceneNode::findChildByPointer(const std::shared_ptr<SceneNode>& child){
    for(auto& c : children_){
        if(c == child){
            return true;
        }

        bool c_desc = child->findChildByPointer(child);
        if(c_desc){
            return true;
        }
    }

    return false;
}

void SceneNode::frame(const Eigen::Affine3f& parent_world_transform_){
    //frame logic for components

    auto transform = worldTransform();
    for(auto& child : children_){
        child->frame(transform);
    }
}

void SceneNode::name(std::string nme){
    name_ = nme;
}

std::list<std::shared_ptr<SceneNode> > SceneNode::findChildren(const std::string& name){
    std::list<std::shared_ptr<SceneNode> > children;

    for(auto& child : children_){
        if(child->name() == name){
            children.push_back(child);
        }

        auto grand_children = child->findChildren(name);
        children.insert(children.end(), grand_children.begin(), grand_children.end());
    }

    return children;
}

bool SceneNode::removeChild(const std::shared_ptr<SceneNode>& child){
    auto child_iter = std::find(children_.begin(), children_.end(), child);
    if(child_iter == children_.end()){
        for(auto& c : children_){
            if(c->removeChild(child)){
                return true;
            }
        }
    }
    else{
        (*child_iter)->parent_ = nullptr;
        children_.erase(child_iter);
        return true;
    }

    return false;
}
