#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include <memory>

#include "engine.h"
#include "scenenode.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

using namespace std;

int main(int argc, char *argv[])
{
    std::shared_ptr<SceneNode> node(new SceneNode());
    std::shared_ptr<SceneNode> child(new SceneNode("first_child"));
    node->addChild(child);
    std::shared_ptr<SceneNode> child2(new SceneNode("second_child"));
    node->addChild(child2);
    std::shared_ptr<SceneNode> grand_child(new SceneNode("grand_child"));
    child->addChild(grand_child);
    std::shared_ptr<SceneNode> grand_child2(new SceneNode("first_child"));
    child2->addChild(grand_child2);
    std::shared_ptr<SceneNode> danging_child(new SceneNode());
    child->addChild(danging_child);

    auto found_child = node->findChild("first_child");
    auto found_grandchild = node->findChild("grand_child");
    auto found_dangling = node->findChild("dangle");
    auto found_children = node->findChildren("first_child");

    std::cout << (found_child == child) << std::endl;
    std::cout << (found_grandchild == grand_child) << std::endl;
    std::cout << (found_dangling == danging_child) << std::endl;

    auto found_children_iter = found_children.begin();


    std::cout << (*found_children_iter == child) << std::endl;
    std::advance(found_children_iter, 1);
    std::cout << (*found_children_iter == grand_child2) << std::endl;

    Eigen::Quaternion<float> rot, invrot;
    rot = Eigen::AngleAxis<float>(1.f, Eigen::Vector3f(0, 1, 0));
    invrot = Eigen::AngleAxis<float>(-1.f, Eigen::Vector3f(0, 1, 0));
    node->rotation(rot);
    auto quat = child->worldRotation();

    std::cout << rot.w() << " " << rot.x() << " " << rot.y() << " " << rot.z() << " " << std::endl;
    std::cout << quat.w() << " " << quat.x() << " " << quat.y() << " " << quat.z() << " " << std::endl;

    child->rotateBy(invrot);
    quat = child->worldRotation();
    auto local_rot = child->rotation();

    std::cout << quat.w() << " " << quat.x() << " " << quat.y() << " " << quat.z() << " " << std::endl;
    std::cout << local_rot.w() << " " << local_rot.x() << " " << local_rot.y() << " " << local_rot.z() << " " << std::endl;

    quat = grand_child->worldRotation();
    local_rot = grand_child->rotation();

    std::cout << quat.w() << " " << quat.x() << " " << quat.y() << " " << quat.z() << " " << std::endl;
    std::cout << local_rot.w() << " " << local_rot.x() << " " << local_rot.y() << " " << local_rot.z() << " " << std::endl;

    node->removeChild(child2);
    auto second_child = node->findChild("second_child");

    std::cout << second_child.get() << std::endl;

    child2->translation(Eigen::Vector3f(5.f, 0.f, 1.f));
    Eigen::Vector3f gc2trans = grand_child2->translation();
    Eigen::Vector3f gc2transworld = grand_child2->worldTranslation();
    std::cout << gc2trans(0) << " " << gc2trans(1) << " " << gc2trans(2) << std::endl;
    std::cout << gc2transworld(0) << " " << gc2transworld(1) << " " << gc2transworld(2) << std::endl;
    grand_child2->translation(Eigen::Vector3f(10.f, 0.f, 0.f));
    gc2transworld = grand_child2->worldTranslation();
    std::cout << gc2transworld(0) << " " << gc2transworld(1) << " " << gc2transworld(2) << std::endl;
    child2->rotateBy(rot);
    gc2transworld = grand_child2->worldTranslation();
    std::cout << gc2transworld(0) << " " << gc2transworld(1) << " " << gc2transworld(2) << std::endl;




//    Engine* engine = Engine::engine();

//    engine->startup();
//    engine->run();
//    engine->shutdown();

    return 0;
}

