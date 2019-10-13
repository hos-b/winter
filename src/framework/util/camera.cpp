#include "framework/util/camera.h"
#include <GLFW/glfw3.h>

namespace winter
{
namespace util
{

Camera::Camera(glm::vec3 position, glm::vec3 up, float roll, float pitch, float yaw, float move_speed, float rotation_speed) : 
        position_(position), up_vector_(up), world_up_vector_(glm::vec3(0.0f, 1.0f, 0.0f)), roll_(roll), pitch_(pitch), yaw_(yaw),
        translation_sensitivity_(move_speed), rotation_sensitivity_(rotation_speed)
{
    target_vector_ = glm::vec3(0.0f, 0.0f, -1.0f);
    CameraUpdate();
}

Camera::~Camera()
{
}

void Camera::CameraUpdate()
{
    target_vector_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    target_vector_.y = sin(glm::radians(pitch_));
    target_vector_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    target_vector_ = glm::normalize(target_vector_);

    right_vector_ = glm::normalize(glm::cross(target_vector_, world_up_vector_));
    up_vector_ = glm::normalize(glm::cross(right_vector_, target_vector_));
}

glm::mat4 Camera::view_matrix()
{
    return glm::lookAt(position_, position_+target_vector_, up_vector_);
}

// input events
void Camera::OnKeyboardEvent(int key, int code , int action, int mods)
{
    (void)code;
    (void)mods;
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        position_ += target_vector_ * translation_sensitivity_;
    }
    else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        position_ -= target_vector_ * translation_sensitivity_;
    }
    else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        position_ -= right_vector_ * translation_sensitivity_;
    }
    else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        position_ += right_vector_ * translation_sensitivity_;
    }
}
void Camera::OnMouseKeyEvent(int button, int action, int mods)
{
    (void)button;
    (void)action;
    (void)mods;
}
void Camera::OnMouseMoveEvent(double delta_x, double delta_y)
{
    (void)delta_x;
    (void)delta_y;
}

} // namespace util
} // namespace winter
