#include "scene.h"

Scene::Scene() : root_(new SceneNode("Root")){
}

Scene::~Scene(){

}

bool Scene::addChild(std::shared_ptr<SceneNode> parent, std::shared_ptr<SceneNode> child){
    if(parent == child){
        return false;
    }

    bool parent_found = root_->findChildByPointer(parent);
    if(!parent_found){
        return false;
    }

    root_->removeChild(child);
    parent->addChild(child);
}

std::pair<bool, std::shared_ptr<SceneNode> > Scene::addChild(std::shared_ptr<SceneNode> parent, std::string child_name){
    bool parent_found = root_->findChildByPointer(parent);
    if(!parent_found){
        return std::make_pair(false, nullptr);
    }

    std::shared_ptr<SceneNode> child(new SceneNode(child_name));
    parent->addChild(child);

    return std::make_pair(true, child);
}

std::shared_ptr<SceneNode> Scene::rootNode(){
    return root_;
}

void Scene::frame(){
    root_->frame(Eigen::Affine3f::Identity());
}
