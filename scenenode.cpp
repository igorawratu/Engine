#include "scenenode.h"
#include <type_traits>

SceneNode::SceneNode() : SceneNode("Nameless"){
}

SceneNode::SceneNode(std::string name) : parent_(nullptr), root_(nullptr), name_(name), rotation_(Eigen::Quaternion<float>::Identity()),
                                         translation_(0.f, 0.f, 0.f), scale_(1.f, 1.f, 1.f), components_sorted_(true){
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
    child->root_ = root_;
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

void SceneNode::frame(const Eigen::Affine3f& parent_world_transform){
    if(!components_sorted_){
        components_.sort([](const std::unique_ptr<Component>& first, const std::unique_ptr<Component>& second){return first->priority_ < second->priority_;});
    }

    if(!components_.empty()){
        for(auto iter = components_.begin(); iter != components_.end(); ++iter){
            (**iter).frameStart();
        }

        for(auto iter = components_.rbegin(); iter != components_.rend(); ++iter){
            (**iter).frameEnd();
        }
    }

    auto transform = Eigen::Translation3f(translation_) * rotation_ * Eigen::Scaling(scale_);
    auto world = parent_world_transform * transform;
    for(auto& child : children_){
        child->frame(world);
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

void SceneNode::addComponent(std::unique_ptr<Component>&& component){
    component->owner_ = this;
    component->startup();
    components_.push_back(std::move(component));

    components_sorted_ = false;
}

void SceneNode::removeComponent(Component* component){
    component->shutdown();
    auto iter_pos = std::find_if(components_.begin(), components_.end(),
                                 [&component](const std::unique_ptr<Component>& val){return component == val.get();});

    if(iter_pos != components_.end()){
        components_.erase(iter_pos);
    }
}

template<typename ComponentType>
Component* SceneNode::getComponent(){
    static_assert(std::is_base_of<Component, ComponentType>::value, "ComponentType passed to SceneNode getComponent() is not subclass of Component");

    for(auto& component : components_){
        ComponentType* sub = dynamic_cast<ComponentType*>(component.get());

        if(sub != nullptr){
            return sub;
        }
    }

    return nullptr;
}

template<typename ComponentType>
std::vector<Component*> SceneNode::getComponents(){
    static_assert(std::is_base_of<Component, ComponentType>::value, "ComponentType passed to SceneNode getComponents() is not subclass of Component");

    std::vector<Component*> out;

    for(auto& component : components_){
        ComponentType* sub = dynamic_cast<ComponentType*>(component.get());

        if(sub != nullptr){
            out.push_back(component.get());
        }
    }

    return out;
}
