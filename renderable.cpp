#include "renderable.h"

void Renderable::frameStart(){
    glUseProgram(material_->getShader()->getProgram());

    glBindVertexArray(vao_name_);
    glBindBuffer(GL_ARRAY_BUFFER, mesh_->getVBO());

    glDrawElements(GL_TRIANGLES, mesh_->getNumIndices(), GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderable::frameEnd(){
}

void Renderable::startup(){
}

void Renderable::shutdown(){
}

Renderable::Renderable(std::unique_ptr<Material>&& mat, Mesh* mesh) : material_(std::move(mat)), mesh_(mesh){
    if(material_ == nullptr || mesh_ == nullptr){
        throw RenderableError("Material and Mesh passed must not be null");
    }

    GLuint mesh_vbo = mesh->getVBO();
    GLuint mesh_ibo = mesh->getIBO();

    if(mesh_vbo == 0 || mesh_ibo == 0){
        throw RenderableError("Mesh VBO or IBO not initialized");
    }

    glGenVertexArrays(1, &vao_name_);
    glBindVertexArray(vao_name_);

    if(material_->getPositionLocation() >= 0){
        GLuint loc = (GLuint)mat->getPositionLocation();
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, position));
    }

    if(material_->getTexcoordLocation() >= 0){
        GLuint loc = (GLuint)mat->getTexcoordLocation();
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, texturecoord));
    }

    if(material_->getColourLocation() >= 0){
        GLuint loc = (GLuint)mat->getColourLocation();
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, colour));
    }

    if(material_->getNormalLocation() >= 0){
        GLuint loc = (GLuint)mat->getNormalLocation();
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, normal));
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_ibo);

    glBindVertexArray(0);

}

Renderable::Renderable(std::unique_ptr<Material>&& mat, Mesh* mesh, GLuint vao_name) : material_(std::move(mat)), mesh_(mesh), vao_name_(vao_name){
}

Renderable::Renderable(const Renderable& other) : material_(std::move(other.material_->clone())), mesh_(other.mesh_), vao_name_(other.vao_name_){
}

Renderable& Renderable::operator = (const Renderable& other){
    material_ = std::move(other.material_->clone());
    mesh_ = other.mesh_;
    vao_name_ = other.vao_name_;

    return *this;
}

Renderable::~Renderable(){
}

Material* Renderable::getMaterial(){
    return material_.get();
}

Mesh* Renderable::getMesh(){
    return mesh_;
}

GLuint Renderable::getVAOName(){
    return vao_name_;
}
