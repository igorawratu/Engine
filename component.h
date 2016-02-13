#ifndef COMPONENT_H
#define COMPONENT_H

#include <limits>

class Component
{
protected:
    unsigned int priority_;

protected:
    virtual void frameStart() = 0;
    virtual void frameEnd() = 0;

public:
    Component() : priority_(std::numeric_limits<unsigned int>::max()){
    }

    bool operator == (const Component& other){
        return priority_ == other.priority_;
    }

    bool operator < (const Component& other){
        return priority_ < other.priority_;
    }

    bool operator > (const Component& other){
        return priority_ > other.priority_;
    }
};

#endif // COMPONENT_H
