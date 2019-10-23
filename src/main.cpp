#include "framework/base/vertex_buffer.h"
#include "framework/base/index_buffer.h"
#include "framework/base/vertex_array.h"
#include "framework/base/texture.h"
#include "framework/base/renderer.h"
#include "framework/base/shader.h"
#include "framework/misc/window.h"
#include "framework/misc/debug.h"

#include <unistd.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

int main(void)
{
        winter::util::Window display(800, 600, "winter");
    
    //imgui
    const char* glsl_version = "#version 330";
    ImGui::CreateContext();
    ImGuiIO io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(display.window(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // blending options
    GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLDebug(glEnable(GL_BLEND));

    // vertex & index buffers
    float positions[] = {
       -100.0f, -100.0f, 0.0f, 0.0f,  // 0
        100.0f, -100.0f, 3.0f, 0.0f,  // 1
        100.0f,  100.0f, 3.0f, 3.0f,  // 2
       -100.0f,  100.0f, 0.0f, 3.0f   // 3
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    // vertex array
    winter::base::VertexArray va;
    // vertex buffer
    winter::base::VertexBuffer vb(positions, 4 * 4 *sizeof(float));
    // layouts
    winter::base::VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    // index buffer
    winter::base::IndexBuffer ib(indices, 6);

    // shaders
    char cwd[100];
    getcwd(cwd, 100);
    std::cout << "cwd: " << cwd << std::endl;
    winter::base::Shader shader("../res/shaders/basic/basic_shader.glsl");
    shader.Bind();

    // projection, view, model matrces
    glm::mat4 projection = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
    glm::mat4 view =  glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(150, 150, 0));
    glm::mat4 mvp = projection * view * model;
    shader.SetUniform<glm::mat4, 16>("u_model_view_projection", mvp);

    // textures
    winter::base::Texture texture("../res/textures/fb.png", GL_REPEAT);
    texture.Bind();
    shader.SetUniform<int, 1>("u_texture", 0);

    // unbind everything
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    // vsync
    glfwSwapInterval(1);

    // imgui state
    glm::vec3 translation_a(100, 100, 0);
    glm::vec3 translation_b(400, 100, 0);
    while (display.is_ok())
    {
        winter::base::Renderer::Clear(winter::base::Renderer::RenderMode::GL2D);
        // start the imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // change translation in x direction with imgui
        model = glm::translate(glm::mat4(1.0f), translation_a);
        mvp = projection * view * model;
        

        // draw the current bound index buffer of type uint, count 6
        // renderer binds both buffer before the draw call
        shader.Bind();
        model = glm::translate(glm::mat4(1.0f), translation_a);
        mvp = projection * view * model;
        shader.SetUniform<glm::mat4, 16>("u_model_view_projection", mvp);
        winter::base::Renderer::Draw(va, ib, shader);

        model = glm::translate(glm::mat4(1.0f), translation_b);
        mvp = projection * view * model;
        shader.SetUniform<glm::mat4, 16>("u_model_view_projection", mvp);
        winter::base::Renderer::Draw(va, ib, shader);

        // render imgui stuff
        ImGui::Begin("model translate");
        ImGui::SliderFloat3("Translation A", &translation_a.x, 0.0f, 640.0f);
        ImGui::SliderFloat3("Translation B", &translation_b.x, 0.0f, 640.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        display.SwapBuffers();
        display.PollEvents();
    }
    // imgui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(NULL);

    return 0;
}