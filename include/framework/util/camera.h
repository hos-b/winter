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
    Camera(glm::vec3 position, glm::vec3 up, float roll, float pitch, float yaw, float move_speed, float rotation_speed);
    ~Camera() override;

    void OnKeyboardEvent(int key, int code , int action, int mods) override;
    void OnMouseKeyEvent(int button, int action, int mods) override;
    void OnMouseMoveEvent(double delta_x, double delta_y) override;
    void CameraUpdate();
    glm::mat4 view_matrix();
private:
    glm::vec3 position_;
    glm::vec3 up_vector_;
    glm::vec3 target_vector_;
    glm::vec3 right_vector_;
    glm::vec3 world_up_vector_;

    float roll_, pitch_, yaw_;

    float translation_sensitivity_;
    float rotation_sensitivity_;
};

} // namespace util
} // namespace winter

#endif