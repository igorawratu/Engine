#include "resourcemanager.h"

std::unique_ptr<ResourceManager> ResourceManager::resource_manager_ = nullptr;

ResourceManager* ResourceManager::resourceManager(){
    return ResourceManager::resource_manager_.get();
}

bool ResourceManager::initialize(){
    if(resource_manager_ != nullptr){
        return false;
    }

    ResourceManager::resource_manager_ = std::unique_ptr<ResourceManager>(new ResourceManager);

    return true;
}

bool ResourceManager::shutdown(){
    if(resource_manager_ == nullptr){
        return false;
    }

    ResourceManager::resource_manager_ = nullptr;

    return true;
}

ResourceManager::ResourceManager() : mesh_id_counter_(0), shader_id_counter_(0){

}

ResourceManager::~ResourceManager(){

}

Mesh* ResourceManager::createMesh(const std::string& lexical_name, std::unique_ptr<std::vector<VertexData> >&& vertices, std::unique_ptr<std::vector<GLuint> >&& indices,
                              MeshCacheOption cache_options){
    if(mesh_lexical_names_.find(lexical_name) != mesh_lexical_names_.end()){
        std::cerr << "Error: Mesh lexical names must not be duplicate" << std::endl;

        return nullptr;
    }

    std::uint32_t mesh_id = mesh_id_counter_++;

    meshes_[mesh_id] = std::unique_ptr<Mesh>(new Mesh(lexical_name, mesh_id, cache_options));

    if(vertices != nullptr && indices != nullptr){
        meshes_[mesh_id]->setMeshData(std::move(vertices), std::move(indices));
    }

    mesh_lexical_names_[lexical_name] = mesh_id;

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

Mesh* ResourceManager::getMesh(const std::string& lexical_name){
    if(mesh_lexical_names_.find(lexical_name) != mesh_lexical_names_.end()){
        std::uint32_t id = mesh_lexical_names_[lexical_name];
        return meshes_[id].get();
    }
    else{
        return nullptr;
    }
}

bool ResourceManager::freeMesh(const std::uint32_t& id){
    if(meshes_.find(id) != meshes_.end()){
        std::string lexical_name = meshes_[id]->getLexicalName();

        meshes_.erase(id);
        mesh_lexical_names_.erase(lexical_name);

        return true;
    }
    else{
        return false;
    }
}

Shader* ResourceManager::createShader(const std::string& lexical_name, const std::string& vs, const std::string& fs, ShaderDataType data_type){
    if(shader_lexical_names_.find(lexical_name) != shader_lexical_names_.end()){
        std::cerr << "Error: Shader lexical names must not be duplicate" << std::endl;
        return nullptr;
    }
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
            shader = std::unique_ptr<Shader>(new Shader(lexical_name, id, vs_dat, fs_dat));
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
            shader = std::unique_ptr<Shader>(new Shader(lexical_name, id, vs, fs));
        }
        catch(ShaderCompileError e){
            std::cerr << "Error: " << e.what() << std::endl;
            return nullptr;
        }

        shaders_[id] = std::move(shader);
    }

    shader_lexical_names_[lexical_name] = id;
}

Shader* ResourceManager::getShader(const std::uint32_t& id){
    if(shaders_.find(id) != shaders_.end()){
        return shaders_[id].get();
    }
    else{
        return nullptr;
    }
}

Shader* ResourceManager::getShader(const std::string& lexical_name){
    if(shader_lexical_names_.find(lexical_name) != shader_lexical_names_.end()){
        std::uint32_t id = shader_lexical_names_[lexical_name];

        return shaders_[id].get();
    }
    else{
        return nullptr;
    }
}

bool ResourceManager::freeShader(const std::uint32_t& id){
    if(shaders_.find(id) != shaders_.end()){
        std::string lexical_name = shaders_[id]->getLexicalName();
        shader_lexical_names_.erase(lexical_name);
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

    if(existing_vaos_.find(hash) != existing_vaos_.end()){
        GLuint vao_name = existing_vaos_[hash];

        try{
            return std::unique_ptr<Renderable>(new Renderable(std::move(mat), mesh, vao_name));
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
