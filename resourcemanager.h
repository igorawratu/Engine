#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <memory>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>

#include "mesh.h"
#include "shader.h"
#include "renderable.h"

enum ShaderDataType{SHADER_FILE, SHADER_RAW};

class ResourceManager
{
friend std::unique_ptr<ResourceManager>::deleter_type;
friend class Engine;
private:
    std::uint32_t mesh_id_counter_;
    std::unordered_map<std::uint32_t, std::unique_ptr<Mesh> > meshes_;
    std::unordered_map<std::string, std::uint32_t> mesh_lexical_names_;

    std::uint32_t shader_id_counter_;
    std::unordered_map<std::uint32_t, std::unique_ptr<Shader> > shaders_;
    std::unordered_map<std::string, std::uint32_t> shader_lexical_names_;

    std::unordered_map<std::uint64_t, GLuint> existing_vaos_;

    static std::unique_ptr<ResourceManager> resource_manager_;

private:
    //only constructable by engine
    ResourceManager();

    static bool initialize();
    static bool shutdown();

public:
    static ResourceManager* resourceManager();

    //no copy allowed
    ResourceManager(const ResourceManager& other) = delete;
    ResourceManager& operator=(const ResourceManager& other) = delete;
    ~ResourceManager();

    /**
     * @brief Creates a mesh with the specified lexical name, vertices, indices, and cache options. If either vertices or indices are nullptr, both are not assigned. However,
     * they can be later assigned.
     * @param lexical_name The lexical name of the mesh. NB: This should be unique! as it is used for lookup purposes. If it is not unique, looking up the mesh with the specified
     * lexical name will return the last mesh with that name.
     * @param vertices vertices of mesh, default value is nullptr
     * @param indices indices of mesh, default value is nullptr
     * @param cache_options caching options of mesh, can be either CACHE, or DELETE_ON_BUFFER_CREATION. Default value is DELETE_ON_BUFFER_CREATION.
     * @return a pointer to the created mesh. The ownership is still held by MeshManager
     */
    Mesh* createMesh(const std::string& lexical_name, std::unique_ptr<std::vector<VertexData> >&& vertices = nullptr, std::unique_ptr<std::vector<GLuint> >&& indices = nullptr,
                     MeshCacheOption cache_options = DELETE_ON_BUFFER_CREATION);

    /**
     * @brief Gets the mesh with the specified /p id
     * @param id ID of the mesh to search for
     * @return observer pointer to the mesh requested, or nullptr if not found
     */
    Mesh* getMesh(const std::uint32_t& id);

    /**
     * @brief Gets the mesh with the specified /p lexical_name
     * @param lexical_name Lexical name of the mesh
     * @return observer pointer to the mesh requested, or nullptr if not found
     */
    Mesh* getMesh(const std::string& lexical_name);

    /**
     * @brief Deallocates mesh data of the mesh with specified /p id from both CPU and GPU.
     * @param id ID of the mesh to deallocate
     * @return true if succeeded, false if Mesh not found in manager
     */
    bool freeMesh(const std::uint32_t& id);

    /**
     * @brief Creates a shader with either specified file names, or actual shader code. The type of data passed is specified with /p data_type,
     * with SHADER_FILE being from file, and SHADER_RAW being actual shader code. The shader is also assigned the specified lexical name
     * @param lexical_name The lexical name of the shader. NB: This should be unique! as it is used for lookup purposes. If it is not unique, looking up the shader with the specified
     * lexical name will return the last shader with that name.
     * @param vs Either The file name containing the vertex shader, or the vertex shader code
     * @param fs Either The file name containing the fragment shader, or the fragment shader code
     * @param data_type Type of the data strings passed through. SHADER_FILE for filenames, SHADER_RAW for actual shader code.
     * @return observer pointer to the shader created
     */
    Shader* createShader(const std::string& lexical_name, const std::string& vs, const std::string& fs, ShaderDataType data_type);

    /**
     * @brief Gets the shader with the specified \p id
     * @param id ID of the shader to return
     * @return observer pointer to shader, or nullptr if not found
     */
    Shader* getShader(const std::uint32_t& id);

    /**
     * @brief Gets a shader with the specified /p lexical_name
     * @param lexical_name lexical name of the shader
     * @return observer pointer to shader, or nullptr if not found
     */
    Shader* getShader(const std::string& lexical_name);

    /**
     * @brief Deallocates shader program with the specified \p id
     * @param id ID of the shader to delete
     * @return true if success, otherwise false
     */
    bool freeShader(const std::uint32_t& id);

    /**
     * @brief Creates and returns a renderable with the given material and mesh. The material's ownership is transfered to the renderable,
     * whereas the mesh is not as it is merely an observer pointer. This method is used to create a renderable instead of allowing users
     * to directly create them as the ResourceManager wants to keep track of existing VAOs, and thus will assign a relevant existing vao to the
     * Renderable should it exist, instead of creating a new one. This will reduce the number of VAO switches during rendering.
     * @param mat Material of the renderable
     * @param mesh Mesh of the renderable
     * @return a std::unique_ptr containing the created renderable, or nullptr should an error have occurred.
     */
    std::unique_ptr<Renderable> createRenderable(std::unique_ptr<Material>&& mat, Mesh* mesh);
};

#endif // RESOURCEMANAGER_H
