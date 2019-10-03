#include "renderer.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "shader.h"
#include "gl_utils.h"
#include "glf_texture.h"

#include <unistd.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
    GLFWwindow* window =  BoilerPlate(640, 480);
    
    //imgui
    const char* glsl_version = "#version 330";
    ImGui::CreateContext();
    ImGuiIO io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // blending options
    GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLDebug(glEnable(GL_BLEND));

    // vertex & index buffers
    float positions[] = {
        100.0f, 100.0f, 0.0f, 0.0f,  // 0
        300.0f, 100.0f, 3.0f, 0.0f,  // 1
        300.0f, 300.0f, 3.0f, 3.0f,  // 2
        100.0f, 300.0f, 0.0f, 3.0f   // 3
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    // vertex array
    VertexArray va;
    // vertex buffer
    VertexBuffer vb(positions, 4 * 4 *sizeof(float));
    // layouts
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    // index buffer
    IndexBuffer ib(indices, 6);

    // shaders
    char cwd[100];
    getcwd(cwd, 100);
    std::cout << "cwd: " << cwd << std::endl;
    Shader shader("../res/shaders/basic_shader.glsl");
    shader.Bind();

    // projection, view, model matrces
    glm::mat4 projection = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
    glm::mat4 view =  glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(150, 150, 0));
    glm::mat4 mvp = projection * view * model;
    shader.SetUniform4x4f("u_model_view_projection", mvp);

    // textures
    Texture texture("../res/textures/fb.png", GL_REPEAT);
    texture.Bind();
    shader.SetUniform1i("u_texture", 0);

    // unbind everything
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;

    // vsync
    glfwSwapInterval(1);

    float r = 0.0f;

    // imgui state
    glm::vec3 translation(150, 150, 0);
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        // start the imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // shader should be bound for uniforms
        shader.Bind();
        shader.SetUniform4f("u_color", r, 0.3f, 1.0f, 1.0f);

        // change translation in x direction with imgui
        model = glm::translate(glm::mat4(1.0f), translation);
        mvp = projection * view * model;
        shader.SetUniform4x4f("u_model_view_projection", mvp);

        va.Bind();
        ib.Bind();
        // draw the current bound index buffer of type uint, count 6
        renderer.Draw(va, ib, shader);

        // render imgui stuff
        ImGui::Begin("model translate");
        ImGui::SliderFloat3("float", &translation.x, 0.0f, 640.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLDebug(glfwSwapBuffers(window));
        GLDebug(glfwPollEvents());
    }
    // imgui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(NULL);

    glfwTerminate();
    return 0;
}