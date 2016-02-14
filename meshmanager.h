#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include <memory>
#include "mesh.h"

class MeshManager
{
friend class Engine;
public:
    MeshManager();
    MeshManager(const MeshManager& other) = delete;
    ~MeshManager();
};

#endif // MESHMANAGER_H
