#include "mesh.h"

Mesh::Mesh(const std::string& lexical_name, std::uint32_t id, MeshCacheOption cache_option) :  id_(id), vbo_name_(0), ibo_name_(0), indices_(nullptr),
                                                                                        vertices_(nullptr), cache_option_(cache_option),
                                                                                        initialized_(false), lexical_name_(lexical_name){
}

Mesh::~Mesh(){
    if(vbo_name_ != 0){
        glDeleteBuffers(1, &vbo_name_);
    }

    if(ibo_name_ != 0){
        glDeleteBuffers(1, &ibo_name_);
    }
}

GLuint Mesh::getVBO(){
    if(vbo_name_ != 0){
        return vbo_name_;
    }

    if(vertices_ == nullptr || indices_ == nullptr){
        return 0;
    }

    initializeBuffers();

    return vbo_name_;
}

GLuint Mesh::getIBO(){
    if(ibo_name_ != 0){
        return ibo_name_;
    }

    if(vertices_ == nullptr || indices_ == nullptr){
        return 0;
    }

    initializeBuffers();

    return ibo_name_;
}

std::uint32_t Mesh::getID(){
    return id_;
}

std::vector<GLuint>* Mesh::getIndices(){
    return indices_.get();
}

std::string Mesh::getLexicalName(){
    return lexical_name_;
}

std::vector<VertexData>* Mesh::getVertices(){
    return vertices_.get();
}

bool Mesh::setMeshData(std::unique_ptr<std::vector<VertexData> >&& vertices, std::unique_ptr<std::vector<GLuint> >&& indices){
    if(initialized_){
        return false;
    }

    vertices_ = std::move(vertices);
    indices_ = std::move(indices);

    num_indices_ = indices_->size();
    num_vertices_ = vertices->size();

    return true;
}

void Mesh::initializeBuffers(){
    assert(vertices_ != nullptr && indices_ != nullptr);

    glGenBuffers(1, &vbo_name_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_name_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertices_->size(), &(*vertices_)[0], GL_STATIC_DRAW);

    glGenBuffers(1, &ibo_name_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_name_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices_->size(), &(*indices_)[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if(cache_option_ == DELETE_ON_BUFFER_CREATION){
        vertices_ = nullptr;
        indices_ = nullptr;
    }

    initialized_ = true;
}

size_t Mesh::getNumIndices(){
    return num_indices_;
}

size_t Mesh::getNumVertices(){
    return num_vertices_;
}
