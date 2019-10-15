#ifndef __WINTER_CAMERA__
#define __WINTER_CAMERA__

#include "framework/base/abstract_types.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace winter
{
namespace util
{

class Camera : public base::InputSubscriber
{
public:
    Camera(glm::vec3 position, glm::vec3 up, float roll, float pitch, float yaw);
    Camera(glm::vec3 position, glm::vec3 up, glm::vec3 target);
    ~Camera() override;

    void OnKeyboardEvent(int key, int code , int action, int mods, float delta_time) override;
    void OnMouseKeyEvent(int button, int action, int mods, float delta_time) override;
    void OnMouseMoveEvent(double delta_x, double delta_y, float delta_time) override;
    void SetSensitivity(float translation, float rotation);
    glm::mat4 view_matrix();

    bool mouse_enabled() { return mouse_enabled_; }

private:
    void RefreshCoordinates();
    void UpdateDirection();

    glm::vec3 position_;
    glm::vec3 up_vector_;
    glm::vec3 world_up_vector_;
    glm::vec3 direction_vector_;
    glm::vec3 right_vector_;
    float roll_, pitch_, yaw_;
    float translation_sensitivity_;
    float rotation_sensitivity_;
    bool mouse_enabled_;
};

} // namespace util
} // namespace winter

#endif