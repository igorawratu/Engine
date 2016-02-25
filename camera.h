#ifndef CAMERA_H
#define CAMERA_H

#include "component.h"

#include <Eigen/Geometry>
#include <math.h>

/**
 * @brief The Viewport struct defines the viewport coordinates between 0 and 1, with 0,0 being the bottom right of the window, and 1,1 being the top left
 */
struct Viewport{
    std::pair<double, double> start;
    std::pair<double, double> end;

    Viewport();
};

enum ProjectionMode{ORTHO, PERSPECTIVE};

class Camera : public Component
{
private:
    Viewport viewport_;
    ProjectionMode proj_mode_;
    std::pair<float, float> far_near_;
    float fov_;

private:
    //helpers for projectionMatrix
    Eigen::Matrix4f calcPerspectiveMat();
    Eigen::Matrix4f calcOrthoMat(const std::pair<std::uint32_t, std::uint32_t>& viewport_res);

protected:
    virtual void frameStart();
    virtual void frameEnd();
    virtual void startup();
    virtual void shutdown();

public:
    Camera();
    Camera(const Viewport& viewport, ProjectionMode proj_mode, float far, float near, float fov);
    Camera& operator = (const Camera& other);
    Camera(const Camera& other);
    ~Camera();

    /**
     * @brief Calculates and gets projection matrix of the camera. This matrix will either be orthographic, or perspective, depending on the mode
     * @param viewport_res Resolution of viewport of camera. This is primarily used for orthographic cameras
     * @return projection matrix of camera as a 4x4 Eigen float matrix
     */
    Eigen::Matrix4f projectionMatrix(const std::pair<std::uint32_t, std::uint32_t>& viewport_res);

    /**
     * @brief Calculates and gets view matrix of the camera. This is essentially just the inverse of the world matrix of the scene node.
     * @return view matrix of camera as a 4x4 Eigen float matrix
     */
    Eigen::Matrix4f viewMatrix();

    /**
     * @brief Sets projection mode of camera. Either ORTHO or PERSPECTIVE.
     * @param proj_mode projection mode to be set
     */
    void setProjectionMode(ProjectionMode proj_mode);

    /**
     * @brief Sets viewport information for camera. Instead of specifying the absolute number of pixels, the coordinate space is instead
     * between 0 and 1, with 0,0 being botton left, and 1,1 being top right. The reasoning for this is to allow for window resizes without
     * actually stretching the image rendered.
     * @param viewport Viewport to be set. The first coordinate must be smaller than the second, otherwise this method fails.
     * @return true if succeeded, otherwise false
     */
    bool setViewport(const Viewport& viewport);

    /**
     * @brief Sets the far and near plane of the projection. Far must be larger than near, and both must be non-zero non-negative.
     * @param far far plane
     * @param near near plane
     * @return true of success, otherwise false
     */
    bool setFarnear(float far, float near);

    /**
     * @brief Sets field of view along the Y axis.
     * @param fov The angle of the field of view in radians. Must be larger than 0 and smaller than PI
     * @return true of success, otherwise false
     */
    bool setFoV(float fov);

    /**
     * @brief Gets viewport info of the camera
     * @return camera's viewport
     */
    Viewport getViewport();

    /**
     * @brief Gets Y-axis field of view of the camera
     * @return field of view in radius as a float
     */
    float getFoV();

    /**
     * @brief Gets far and near plane of the camera
     * @return a pair with first element being far, and second element being near
     */
    std::pair<float, float> getFarnear();

    /**
     * @brief Gets projection mode of the camera
     * @return projection mode
     */
    ProjectionMode getProjectionMode();

    virtual std::unique_ptr<Component> clone();
};

#endif // CAMERA_H
