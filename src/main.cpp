#include "gl_utils.h"
#include "renderer.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "shader.h"
#include "glf_texture.h"
#include <unistd.h>

int main(void)
{
    GLFWwindow* window =  BoilerPlate();
    GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLDebug(glEnable(GL_BLEND));
    float positions[] = {
        -0.5f,-0.5f, 0.0f, 0.0f,  // 0
         0.5f,-0.5f, 1.0f, 0.0f,  // 1
         0.5f, 0.5f, 1.0f, 1.0f,  // 2
        -0.5f, 0.5f, 0.0f, 1.0f   // 3
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
    
    // textures
    Texture texture("../res/textures/fb.png");
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
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        if (r > 1.0f)
            increment = -0.05f;
        if (r < 0.0f)
            increment = 0.05f;
        r += increment;
        

        // the only way to do it right now
        shader.Bind();
        shader.SetUniform4f("u_color", r, 0.3f, 1.0f, 1.0f);
        
        va.Bind();
        ib.Bind();
        renderer.Clear();
        // draw the current bound index buffer of type uint, count 6
        renderer.Draw(va, ib, shader);
        GLDebug(glfwSwapBuffers(window));
        GLDebug(glfwPollEvents());
    }
    glfwTerminate();
    return 0;
}