#include "meshmanager.h"

MeshManager::MeshManager() : mesh_id_counter_(0){

}

MeshManager::~MeshManager(){

}

Mesh* MeshManager::createMesh(std::unique_ptr<std::vector<VertexData> >&& vertices, std::unique_ptr<std::vector<GLuint> >&& indices,
                              MeshCacheOption cache_options){
    std::uint32_t mesh_id = mesh_id_counter_++;

    meshes_[mesh_id] = std::unique_ptr<Mesh>(new Mesh(mesh_id, cache_options));

    if(vertices != nullptr && indices != nullptr){
        meshes_[mesh_id]->setMeshData(std::move(vertices), std::move(indices));
    }

    return meshes_[mesh_id].get();
}

Mesh* MeshManager::getMesh(const std::uint32_t& id){
    if(meshes_.find(id) != meshes_.end()){
        return meshes_[id].get();
    }
    else{
        return nullptr;
    }
}

bool MeshManager::freeMesh(const std::uint32_t& id){
    if(meshes_.find(id) != meshes_.end()){
        meshes_.erase(id);
        return true;
    }
    else{
        return false;
    }
}
