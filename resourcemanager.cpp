#include "resourcemanager.h"

ResourceManager::ResourceManager() : mesh_id_counter_(0), shader_id_counter_(0){

}

ResourceManager::~ResourceManager(){

}

Mesh* ResourceManager::createMesh(std::unique_ptr<std::vector<VertexData> >&& vertices, std::unique_ptr<std::vector<GLuint> >&& indices,
                              MeshCacheOption cache_options){
    std::uint32_t mesh_id = mesh_id_counter_++;

    meshes_[mesh_id] = std::unique_ptr<Mesh>(new Mesh(mesh_id, cache_options));

    if(vertices != nullptr && indices != nullptr){
        meshes_[mesh_id]->setMeshData(std::move(vertices), std::move(indices));
    }

    return meshes_[mesh_id].get();
}

Mesh* ResourceManager::getMesh(const std::uint32_t& id){
    if(meshes_.find(id) != meshes_.end()){
        return meshes_[id].get();
    }
    else{
        return nullptr;
    }
}

bool ResourceManager::freeMesh(const std::uint32_t& id){
    if(meshes_.find(id) != meshes_.end()){
        meshes_.erase(id);
        return true;
    }
    else{
        return false;
    }
}

Shader* ResourceManager::createShader(const std::string& vs, const std::string& fs, ShaderDataType data_type){
    std::uint32_t id = shader_id_counter_++;

    if(data_type == SHADER_FILE){
        std::ifstream vs_file(vs);
        if(!vs_file.is_open()){
            std::cerr << "Error: unable to find vertex shader file" << std::endl;
            return nullptr;
        }

        std::stringstream vs_stream;
        vs_stream << vs_file.rdbuf();
        std::string vs_dat = vs_stream.str();

        std::ifstream fs_file(fs);
        if(!fs_file.is_open()){
            std::cerr << "Error: unable to find fragment shader file" << std::endl;
            return nullptr;
        }

        std::stringstream fs_stream;
        fs_stream << fs_file.rdbuf();
        std::string fs_dat = vs_stream.str();

        std::unique_ptr<Shader> shader;
        try{
            shader = std::unique_ptr<Shader>(new Shader(id, vs_dat, fs_dat));
        }
        catch(ShaderCompileError e){
            std::cerr << "Error: " << e.what() << std::endl;
            return nullptr;
        }

        shaders_[id] = std::move(shader);
    }
    else{
        std::unique_ptr<Shader> shader;
        try{
            shader = std::unique_ptr<Shader>(new Shader(id, vs, fs));
        }
        catch(ShaderCompileError e){
            std::cerr << "Error: " << e.what() << std::endl;
            return nullptr;
        }

        shaders_[id] = std::move(shader);
    }
}

Shader* ResourceManager::getShader(const std::uint32_t& id){
    if(shaders_.find(id) != shaders_.end()){
        return shaders_[id].get();
    }
    else{
        return nullptr;
    }
}

bool ResourceManager::freeShader(const std::uint32_t& id){
    if(shaders_.find(id) != shaders_.end()){
        shaders_.erase(id);
        return true;
    }
    else{
        return false;
    }
}

std::unique_ptr<Renderable> ResourceManager::createRenderable(std::unique_ptr<Material>&& mat, Mesh* mesh){
    assert(mat != nullptr && mesh != nullptr);

    std::uint64_t shader_id = (std::uint64_t)mat->getShader()->getID();
    std::uint64_t mesh_id = (std::uint64_t)mesh->getID();

    std::uint64_t hash = (shader_id << 32) | mesh_id;

    if(existing_vaos_.find(hash)){
        GLuint vao_name = existing_vaos_[hash];

        try{
            return std::unique_ptr<Renderable>(std::move(mat), mesh, vao_name);
        }
        catch(RenderableError e){
            std::cerr << e.what() << std::endl;

            return nullptr;
        }
    }

    try{
        std::unique_ptr<Renderable> renderable(new Renderable(std::move(mat), mesh));
        existing_vaos_[hash] = renderable->getVAOName();

        return renderable;
    }
    catch(RenderableError e){
        std::cerr << e.what() << std::endl;

        return nullptr;
    }


}
