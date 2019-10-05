#include "tests/clearcolor.h"
#include "framework/renderer.h"

namespace test{

ClearColorTest::ClearColorTest() : color_{0.2f, 0.3f, 0.8f, 1.0f}
{}
void ClearColorTest::OnRender()
{
    GLDebug(glClearColor(color_[0], color_[1], color_[2], color_[3]));
    GLDebug(glClear(GL_COLOR_BUFFER_BIT));
}
void ClearColorTest::OnImGuiRender()
{
    ImGui::ColorEdit4("clear color", color_);
}

}