#ifndef COMPONENT_H
#define COMPONENT_H

#include <limits>

class Component
{
friend class SceneNode;
protected:
    unsigned int priority_;

protected:
    virtual void frameStart() = 0;
    virtual void frameEnd() = 0;
    virtual void startup() = 0;
    virtual void shutdown() = 0;

public:
    Component() : priority_(std::numeric_limits<unsigned int>::max()){
    }
};

#endif // COMPONENT_H
