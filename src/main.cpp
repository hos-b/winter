#include "gl_utils.h"


    
int main(void)
{
    GLFWwindow* window =  BoilerPlate();
    
    float positions[] = {
        -0.5f,-0.5f,  // 0
         0.5f,-0.5f,  // 1
         0.5f, 0.5f,  // 2
        -0.5f, 0.5f   // 3
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    // vertex array
    unsigned int vao;
    GLDebug(glGenVertexArrays(1, &vao));
    glBindVertexArray(vao);

    // vertex buffer
    unsigned int buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), positions, GL_STATIC_DRAW);

    // defining position attribute
    GLDebug(glEnableVertexAttribArray(0));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (const void*)0);

    // index buffer
    unsigned int index_buffer_id;
    glGenBuffers(1, &index_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // shaders
    std::string vertex_shader = ReadShaderFromFile("../res/shaders/basic_vertex.glsl");
    std::string fragment_shader = ReadShaderFromFile("../res/shaders/basic_fragment.glsl");
    unsigned int shader = CreateShaders(vertex_shader, fragment_shader);
    glUseProgram(shader);
    glfwSwapInterval(1);
    int location = glGetUniformLocation(shader, "u_color");
    float r = 0.0f;
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        if (r > 1.0f)
            increment = -0.05f;
        if (r < 0.0f)
            increment = 0.05f;
        r += increment;

        glClear(GL_COLOR_BUFFER_BIT);
        glUniform4f(location, r, 0.3f, 1.0f, 1.0f);
        GLDebug(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}