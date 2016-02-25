#include "scene.h"

Scene::Scene() : root_(new SceneNode("Root")){
}

Scene::~Scene(){

}

SceneNode* Scene::rootNode(){
    return root_.get();
}

void Scene::frame(){
    root_->frame(Eigen::Affine3f::Identity());
}
