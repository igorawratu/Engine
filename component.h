#ifndef COMPONENT_H
#define COMPONENT_H

#include <limits>
#include <memory>

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

    Component(const Component& other) : priority_(other.priority_), owner_(other.owner_){
    }

    Component& operator = (const Component& other){
        priority_ = other.priority_;
        owner_ = other.owner_;

        return *this;
    }

    virtual std::unique_ptr<Component> clone() = 0;
};

#endif // COMPONENT_H
