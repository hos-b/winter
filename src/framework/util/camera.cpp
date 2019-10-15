#include "framework/util/camera.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace winter
{
namespace util
{

Camera::Camera(glm::vec3 position, glm::vec3 up, float roll, float pitch, float yaw) : 
        position_(position), world_up_vector_(up), roll_(roll), pitch_(pitch), yaw_(yaw),
        translation_sensitivity_(5.0f), rotation_sensitivity_(5.0f), mouse_enabled_(false)
{
    UpdateDirection();
    RefreshCoordinates();
}
Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 target) :
        position_(position), up_vector_(up), world_up_vector_(up), direction_vector_(target),
        translation_sensitivity_(5.0f), rotation_sensitivity_(5.0f), mouse_enabled_(false)
{
    // camera pointing at negative z
    direction_vector_ = position - target;
    direction_vector_ = glm::normalize(direction_vector_);
    RefreshCoordinates();
}

void Camera::UpdateDirection()
{
    direction_vector_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    direction_vector_.y = sin(glm::radians(pitch_));
    direction_vector_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    direction_vector_ = glm::normalize(direction_vector_);
}

void Camera::RefreshCoordinates()
{
    right_vector_ = glm::normalize(glm::cross(direction_vector_, world_up_vector_));
    up_vector_ = glm::normalize(glm::cross(right_vector_, direction_vector_));
}

Camera::~Camera()
{
}

void Camera::SetSensitivity(float translation, float rotation)
{
    translation_sensitivity_ = translation;
    rotation_sensitivity_ = rotation;
}


glm::mat4 Camera::view_matrix()
{
    return glm::lookAt(position_, position_ + direction_vector_, world_up_vector_);
}

// input events
void Camera::OnKeyboardEvent(int key, int code , int action, int mods, float delta_time)
{
    (void)code;
    (void)mods;
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        position_ += direction_vector_ * translation_sensitivity_ * delta_time;
    }
    else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        position_ -= direction_vector_ * translation_sensitivity_ * delta_time;
    }
    else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        position_ -= right_vector_ * translation_sensitivity_ * delta_time;
    }
    else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        position_ += right_vector_ * translation_sensitivity_ * delta_time;
    }
    else if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        mouse_enabled_ = !mouse_enabled_;
        std::cout << "mouse " << (mouse_enabled_ ? "enabled" : "disabled") << std::endl;
    }
}
void Camera::OnMouseKeyEvent(int button, int action, int mods, float delta_time)
{
    (void)button;
    (void)action;
    (void)mods;
    (void)delta_time;
}
void Camera::OnMouseMoveEvent(double delta_x, double delta_y, float delta_time)
{
    if (!mouse_enabled_)
        return;
    yaw_ += delta_x * delta_time * rotation_sensitivity_;
    pitch_ += delta_y * delta_time * rotation_sensitivity_;
    
    // limit pitch
    if (pitch_ > 89.0f)
        pitch_ = 89.0f;
    else if (pitch_ < -89.0f)
        pitch_ = -89.0f;
    
    UpdateDirection();
    RefreshCoordinates();
}

} // namespace util
} // namespace winter
