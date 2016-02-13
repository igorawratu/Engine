#ifndef COMPONENT_H
#define COMPONENT_H

#include <limits>

class SceneNode;

class Component
{
friend class SceneNode;
protected:
    unsigned int priority_;
    SceneNode* owner_;

protected:
    virtual void frameStart() = 0;
    virtual void frameEnd() = 0;
    virtual void startup() = 0;
    virtual void shutdown() = 0;

public:
    Component() : priority_(std::numeric_limits<unsigned int>::max()), owner_(nullptr){
    }
};

#endif // COMPONENT_H
