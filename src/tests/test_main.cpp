#include "framework/base/renderer.h"
#include "framework/misc/window.h"
#include "framework/misc/debug.h"

#include <unistd.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "tests/clearcolor.h"
#include "tests/texture.h"
#include "tests/pyramid.h"
#include "tests/lighting.h"

int main(void)
{
    winter::util::Window window(1366, 768, "unit tests");
    
    //imgui
    const char* glsl_version = "#version 330";
    ImGui::CreateContext();
    ImGuiIO io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.glfw_window() , true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // vsync
    glfwSwapInterval(1);

    // tests
    winter::test::Test* current_test = nullptr;
    winter::test::TestMenu* menu = new winter::test::TestMenu(current_test);
    menu->SetWindowReference(&window);
    current_test = menu;

    menu->RegisterTest<winter::test::ClearColorTest>("Clear Color");
    menu->RegisterTest<winter::test::TextureTest>("Texture");
    menu->RegisterTest<winter::test::PyramidTest>("Pyramid");
	menu->RegisterTest<winter::test::LightTest>("Lighting");
    while (window.is_ok())
    {
        GLDebug(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        winter::base::Renderer::Clear(winter::base::Renderer::RenderMode::GL2D);
        
        // start the imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (current_test)
        {
            current_test->OnUpdate(0.0f);
            current_test->OnRender();
            ImGui::Begin("Test");
            if (current_test != menu && ImGui::Button("<-"))
            {
                delete current_test;
                current_test = menu;
            }
            current_test->OnImGuiRender();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.SwapBuffers();
        window.PollEvents();
    }
    
    // test clean up
    delete current_test;
    if (current_test!= menu)
        delete menu;

    // imgui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(NULL);

    return 0;
}