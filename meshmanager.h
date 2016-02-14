#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include <memory>
#include <unordered_map>

#include "mesh.h"

class MeshManager
{
friend class Engine;
private:
    std::uint32_t mesh_id_counter_;
    std::unordered_map<std::uint32_t, std::unique_ptr<Mesh> > meshes_;

private:
    //only constructable by engine
    MeshManager();

public:
    //no copy allowed
    MeshManager(const MeshManager& other) = delete;
    MeshManager& operator=(const MeshManager& other) = delete;
    ~MeshManager();

    /**
     * @brief Creates a mesh with the specified vertices, indices, and cache options. If either vertices or indices are nullptr, both are not assigned. However,
     * they can be later assigned.
     * @param vertices vertices of mesh, default value is nullptr
     * @param indices indices of mesh, default value is nullptr
     * @param cache_options caching options of mesh, can be either CACHE, or DELETE_ON_BUFFER_CREATION. Default value is DELETE_ON_BUFFER_CREATION.
     * @return a pointer to the created mesh. The ownership is still held by MeshManager
     */
    Mesh* createMesh(std::unique_ptr<std::vector<VertexData> >&& vertices = nullptr, std::unique_ptr<std::vector<GLuint> >&& indices = nullptr,
                     MeshCacheOption cache_options = DELETE_ON_BUFFER_CREATION);

    /**
     * @brief Gets the mesh with the specified /p id
     * @param id ID of the mesh to search for
     * @return poiter to the mesh requested, or nullptr if not found
     */
    Mesh* getMesh(const std::uint32_t& id);

    /**
     * @brief Deallocates mesh data of the mesh with specified /p id from both CPU and GPU.
     * @param id ID of the mesh to deallocate
     * @return true of succeeded, false if Mesh not found in manager
     */
    bool freeMesh(const std::uint32_t& id);
};

#endif // MESHMANAGER_H
