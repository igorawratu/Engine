#ifndef MESH_H
#define MESH_H

#include "common.h"

#include <vector>
#include <memory>

struct VertexData{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat colour[3];
    GLfloat texturecoord[2];
};

/**
 * @brief The MeshCacheOption enum specifies vertex and index caching options. DELETE_ON_BUFFER_CREATION sets to delete the data once it has
 * been transfered to the GPU, whereas CACHE keeps the data on the cpu.
 */
enum MeshCacheOption{DELETE_ON_BUFFER_CREATION, CACHE};

class Mesh
{
friend class MeshManager;
private:
    std::uint32_t id_;
    GLuint vbo_name_;
    GLuint ibo_name_;

    std::unique_ptr<std::vector<GLuint> > indices_;
    std::unique_ptr<std::vector<VertexData> > vertices_;

    MeshCacheOption cache_option_;

    bool initialized_;

private:
    Mesh(std::uint32_t id, MeshCacheOption cache_option = DELETE_ON_BUFFER_CREATION);

    void initializeBuffers();

public:
    //dissallow constructing or copying of the mesh, as it should only be constructed within MeshManager
    Mesh() = delete;
    Mesh(const Mesh& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;
    ~Mesh();

    /**
     * @brief Gets the name of the vbo associated with the mesh.
     * @return name of the vbo, otherwise 0 in the case of there being no vertex or index data assigned to the mesh object
     */
    GLuint getVBO();

    /**
     * @brief Gets the name of the ibo associated with the mesh.
     * @return name of the ibo, otherwise 0 in the case of there being no vertex or index data assigned to the mesh object
     */
    GLuint getIBO();

    /**
     * @brief Gets pointer to the vertex data of the mesh
     * @return pointer to the vertex data, or nullptr in the case that either there have been no vertex data assigned, or if
     * the data has been deleted after the data has been sent to the GPU (specify cache_option CACHE to prevent this)
     */
    std::vector<GLuint>* getIndices();

    /**
     * @brief Gets pointer to the index data of the mesh
     * @return pointer to the index data, or nullptr in the case that either there have been no index data assigned, or if
     * the data has been deleted after the data has been sent to the GPU (specify cache_option CACHE to prevent this)
     */
    std::vector<VertexData>* getVertices();

    /**
     * @brief Assigns mesh vertex and index data. This can be done as many times as one wishes as long as the the data has not been sent to the GPU (occurs when
     * getVBO() or get IBO() are first called). This is because the system currently does not support dynamic meshes.
     * @param vertices Vertex data of mesh
     * @param indices Index data of mesh
     * @return true of succeeded, otherwise false
     */
    bool setMeshData(std::unique_ptr<std::vector<VertexData> >&& vertices, std::unique_ptr<std::vector<GLuint> >&& indices);

    /**
     * @brief Gets ID of the mesh. Use this to query the MeshManager if you wish to get a pointer to the mesh.
     * @return id of the mesh.
     */
    std::uint32_t getID();
};

#endif // MESH_H
