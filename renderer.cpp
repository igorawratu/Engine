#include "renderer.h"

#include "renderable.h"
#include "camera.h"
#include "engine.h"
#include "window.h"
#include "material.h"
#include "mesh.h"
#include "scenenode.h"

std::unique_ptr<Renderer> Renderer::renderer_ = nullptr;

Renderer::Renderer(){
}

Renderer::~Renderer(){
}

bool Renderer::initialize(){
    if(Renderer::renderer_ == nullptr){
        Renderer::renderer_ = std::unique_ptr<Renderer>(new Renderer);

        return true;
    }

    return false;
}

bool Renderer::shutdown(){
    if(Renderer::renderer_ == nullptr){
        return false;
    }

    Renderer::renderer_ = nullptr;
    return true;
}

Renderer* Renderer::renderer(){
    return Renderer::renderer_.get();
}

void Renderer::frame(){
    if(cameras_.size() > 1){
        std::sort(cameras_.begin(), cameras_.end(), [](Camera* first, Camera* second){
            auto first_vp = first->getViewport();
            auto second_vp = second->getViewport();

            auto first_area = (first_vp.end.first - first_vp.start.first) * (first_vp.end.second - first_vp.start.second);
            auto second_area = (second_vp.end.first - second_vp.start.first) * (second_vp.end.second - second_vp.start.second);

            return first_area > second_area;
        });
    }

    Window* window = Engine::engine()->window();
    auto res = window->getResolution();
    std::pair<std::uint32_t, std::uint32_t> res_unsigned((std::uint32_t)res.first, (std::uint32_t)res.second);

    for(auto camera : cameras_){
        Eigen::Matrix4f view_mat = camera->viewMatrix();
        Eigen::Matrix4f projection_mat = camera->projectionMatrix(res_unsigned);

        Viewport vp = camera->getViewport();
        GLint start_x = vp.start.first * res_unsigned.first;
        GLint start_y = vp.start.second * res_unsigned.second;
        GLint end_x = vp.end.first * res_unsigned.first;
        GLint end_y = vp.end.second * res_unsigned.second;

        assert(start_x < end_x && start_y < end_y);

        GLuint width = end_x - start_x;
        GLuint height = end_y - start_y;

        glViewport(start_x, start_y, width, height);

        GLuint current_program = 0;
        bool program_changed = true;

        for(auto& vao_renderables : renderables_){
            GLuint vao_name = vao_renderables.first;

            GLuint program = vao_renderables.second.front()->getMaterial()->getShader()->getProgram();

            if(current_program != program){
                current_program = program;
                glUseProgram(program);
            }
            else program_changed = false;

            glBindVertexArray(vao_name);

            for(auto renderable : vao_renderables.second){
                Material* mat = renderable->getMaterial();
                Mesh* mesh = renderable->getMesh();

                assert(renderable->owner_ != nullptr);
                Eigen::Affine3f world = renderable->owner_->worldTransform();

                int world_mat_pos = mat->getModelMatLocation();
                int proj_mat_pos = mat->getProjMatLocation();
                int view_mat_pos = mat->getViewMatLocation();

                mat->bind();

                if(program_changed){
                    if(view_mat_pos >= 0){
                        glUniformMatrix4fv((GLuint)view_mat_pos, 1, GL_FALSE, view_mat.data());
                    }

                    if(proj_mat_pos >= 0){
                        glUniformMatrix4fv((GLuint)proj_mat_pos, 1, GL_FALSE, projection_mat.data());
                    }
                }

                if(world_mat_pos >= 0){
                    glUniformMatrix4fv((GLuint)world_mat_pos, 1, GL_FALSE, world.matrix().data());
                }

                glBindBuffer(GL_ARRAY_BUFFER, mesh->getVBO());

                glDrawElements(GL_TRIANGLES, mesh->getNumIndices(), GL_UNSIGNED_INT, NULL);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }

            glBindVertexArray(0);
        }

    }

    cameras_.clear();
    renderables_.clear();
}

void Renderer::addRenderable(Renderable* renderable){
    GLuint vao = renderable->getVAOName();
    if(renderables_.find(vao) != renderables_.end()){
        renderables_[vao].push_back(renderable);
    }
    else{
        renderables_.emplace(std::make_pair(vao, std::list<Renderable*>()));
        renderables_[vao].push_back(renderable);
    }
}

void Renderer::addCamera(Camera* camera){
    cameras_.push_back(camera);
}
