#include "camera.h"
#include "scenenode.h"
#include "renderer.h"

Viewport::Viewport() : start(0, 0), end(0, 0){
}

Eigen::Matrix4f Camera::calcPerspectiveMat(){
    float aspect = (viewport_.end.first - viewport_.start.first) / (viewport_.end.second - viewport_.start.second);
    float cot = 1 / tan(fov_ / 2);
    float scale_x = cot / aspect;
    float far = far_near_.first;
    float near = far_near_.second;

    Eigen::Matrix4f perspective_mat;
    perspective_mat << scale_x, 0.f, 0.f, 0.f,
                    0.f, cot, 0.f, 0.f,
                    0.f, 0.f, -(far + near) / (far - near), (-2 * far * near) / (far - near),
                    0.f, 0.f, -1.f, 0.f;

    return perspective_mat;
}

Eigen::Matrix4f Camera::calcOrthoMat(const std::pair<std::uint32_t, std::uint32_t>& viewport_res){
    float far = far_near_.first;
    float near = far_near_.second;

    Eigen::Matrix4f ortho_mat;
    ortho_mat << 2.f / (float)viewport_res.first, 0.f, 0.f, 0.f,
              0.f, 2.f / (float)viewport_res.second, 0.f, 0.f,
              0.f, 0.f, 1 / (far - near), -near / (far - near),
              0.f, 0.f, 0.f, 1.f;

    return ortho_mat;
}

void Camera::frameStart(){
    Renderer::renderer()->addCamera(this);
}

void Camera::frameEnd(){
}

void Camera::startup(){
}

void Camera::shutdown(){
}

Camera::Camera() : proj_mode_(ORTHO), far_near_(0.1f, 1.f), fov_(M_PI / 2){
}

Camera::Camera(const Viewport& viewport, ProjectionMode proj_mode, float far, float near, float fov) : viewport_(viewport), proj_mode_(proj_mode),
                                                                                                       far_near_(far, near), fov_(fov){
    assert(viewport.start.first >= 0 && viewport.start.first < viewport.end.first &&
           viewport.start.second >= 0 && viewport.start.second < viewport.end.second &&
           viewport.end.first <= 1 && viewport.end.second <= 1);

    assert(fov < M_PI && fov > 0);

    assert(far < near);
}

Camera& Camera::operator = (const Camera& other){
    Component::operator=(other);
    viewport_ = other.viewport_;
    proj_mode_ = other.proj_mode_;
    far_near_ = other.far_near_;
    fov_ = other.fov_;

    return *this;
}

Camera::Camera(const Camera& other) : Component(other), viewport_(other.viewport_), proj_mode_(other.proj_mode_), far_near_(other.far_near_), fov_(other.fov_){
}

Camera::~Camera(){
}

Eigen::Matrix4f Camera::projectionMatrix(const std::pair<std::uint32_t, std::uint32_t>& viewport_res){
    if(proj_mode_ == ORTHO){
        return calcOrthoMat(viewport_res);
    }

    return calcPerspectiveMat();
}

Eigen::Matrix4f Camera::viewMatrix(){
    return owner_->worldTransform().matrix().inverse();
}

void Camera::setProjectionMode(ProjectionMode proj_mode){
    proj_mode_ = proj_mode;
}

bool Camera::setViewport(const Viewport& viewport){
    if(!(viewport.start.first >= 0 && viewport.start.first < viewport.end.first &&
            viewport.start.second >= 0 && viewport.start.second < viewport.end.second &&
            viewport.end.first <= 1 && viewport.end.second <= 1)){
        return false;
    }

    viewport_ = viewport;
    return true;
}

bool Camera::setFarnear(float far, float near){
    if(far < near){
        return false;
    }

    far_near_ = std::make_pair(far, near);
    return true;
}

bool Camera::setFoV(float fov){
    if(fov >= M_PI || fov <= 0.f){
        return false;
    }

    fov_ = fov;
    return true;
}

Viewport Camera::getViewport(){
    return viewport_;
}

float Camera::getFoV(){
    return fov_;
}

std::pair<float, float> Camera::getFarnear(){
    return far_near_;
}

ProjectionMode Camera::getProjectionMode(){
    return proj_mode_;
}

std::unique_ptr<Component> Camera::clone(){
    return std::unique_ptr<Component>(new Camera(*this));
}
