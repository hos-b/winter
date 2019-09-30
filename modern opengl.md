# Modern OpenGL

## 1. Setting up

### 1.1. libraries
install a lot of libraries
```bash
$ sudo apt-get install build-essential libgl1-mesa-dev
$ sudo apt-get install libglew-dev libsdl2-dev libsdl2-image-dev libglm-dev libfreetype6-dev
$ sudo apt-get install mesa-utils
$ sudo apt-get install libglfw3-dev libglfw3
$ sudo apt-get install libglew-dev
$ sudo apt-get install libgl3w libgl3w-dev
```
use them
```cmake
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS}
                    ${GLUT_INCLUDE_DIRS}
                    ${GLFW_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS}
)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} 
                      ${OPENGL_LIBRARIES} 
                      ${GLEW_LIBRARIES}
                      ${GLUT_LIBRARY}
                      glfw
)
```
### 1.2. boilerplate code
`glfw` takes care of making windows, `glew` finds and links the driver specific declarations. <br>
they both need to be initialized in a speicific order. order of `include` directives also matters
```cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
GLFWwindow* Boilerplate()
{
    GLFWwindow* window;
    if (!glfwInit())
        std::cout << "glfw init failed" << std::endl;
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "failed to create window" << std::endl;
    }
    glfwMakeContextCurrent(window);
    GLenum init_enum = glewInit();
    if (init_enum!= GLEW_OK)
        std::cout << glewGetErrorString(init_enum) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    return window;
}
int main()
{
    GLFWwindow* window = Boilerplate();
    while (!glfwWindowShouldClose(window))
    {
        ...
    }
}
```
## 2. Drawing a Simple Triangle

### 2.1. generating a buffer
everything that's generated gets a unique `uint` identifier. to generate a buffer we use
```cpp
unsigned int buffer_id;
glGenBuffers(1, &buffer_id);
```
### 2.2. selection and allocation
we first bind the generated buffer. we then use `glBufferData` to define its size and use
```cpp
float positions[] = {
        -0.5f,-0.5f,
         0.0f, 0.5f,
         0.5f,-0.5f
    };
glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);
```
to unbound a buffer, we can use `glBindBuffer(GL_ARRAY_BUFFER, 0)`.
#### glBufferData
it fills our buffer with data
```cpp
glBufferData(buffer, size, data, usage)
```
data can be `null`, meaning we will assign the data later. there are two groups that define usage
<br>**frequency of access**<br>
* STREAM :
    The data store contents will be modified once and used at most a few times.
* STATIC :
    The data store contents will be modified once and used many times.
* DYNAMIC :
    The data store contents will be modified repeatedly and used many times.

**nature of access**

* DRAW :
    The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.
* READ :
    The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.
* COPY :
    The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.

#### drawing
```cpp
glDrawArrays(GL_TRIANGLES, 0, 3);
// vertex attribute & shader still needed to actually draw it
```
this draws the currently bound buffer. the arguments are
* type : here we're drawing GL_TRIANGLES
* starting index : starting from the first vertex
* number of indices : we have 3

### 2.3. vertex attributes
it tells opengl how our data is layed out. we need to call it once for each attribute we have in a vertex.
```cpp
glVertexAttribPointer(index, size, type, normalized, stride, pointer)
```
* index : index of an attribute in the buffer
* size : element count of the attribute, can be 1-4
* type : attribute data type
* normalized : whether opengl has to convert data to be from 0.0 to 1.0
* stride : byte offset between **vertices**
* pointer : byte offset of an attribute in each vertex

we also need to enable the attribut by its index. for a simple 2D position attribute buffer we'd use something like this 
```cpp
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (const void*)0);
glEnableVertexAttribArray(0);
```
it doesn't matter where we enable the attribute as long as the buffer is bound.
## 3. Shaders
the code might already be able to draw a triangle. this is because some drivers already provide a default shader. the primary purpose of the 
vertex shader is to define where in the window the vertices are drawn. it gets called once for every vertex that we have.<br>
the fragment shader is called once per every pixel that get rasterized and determines the color of that pixel. this means the fragment shader
gets called way more often and has to be optimized. we can also do some calculations in the vertex shader and pass it on to the fragment shader
to save computation time. a shader can access data from the cpu in the form of opengl buffers and uniforms.


### 3.1. compiling shaders
we could read the shaders from a file. they are stored in `std::string`s but will need to be cast as plain `char*` for opengl to compile them.
```cpp
static unsigned int CompileShader(const std::string& source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // = &source[0]
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result==GL_FALSE)
    {
        int length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char*) alloca(length*sizeof(char)); // dynamic stack memory
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile " << (type==GL_VERTEX_SHADER ? "vertex shader":"fragment shader") << std::endl;
        std::cout << message;
        glDeleteShader(id);
        return 0;
    }
    return id;
}
```
```cpp 
glShaderSource(shader, count, string, length);
```
* shader :specifies the handle of the shader object whose source code is to be replaced.
* count : specifies the number of elements in the string and length arrays.
* string : specifies an array of pointers to strings containing the source code to be loaded into the shader.
* length : specifies an array of string lengths. null_ptr if they are NULL terminated.

this can be regarded as the same stages of compiling c++ code. after compiling both shaders, we link them to the glProgram we created, then link
the program itself and return its id. the id is then passed to `glUseProgram`.
```cpp
static unsigned int CreateShaders(const std::string& vertex_shader, const std::string& fragment_shader)
{
    unsigned int program = glCreateProgram();
    // compiling shaders
    unsigned int vs = CompileShader(vertex_shader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragment_shader, GL_FRAGMENT_SHADER);
    // linking shaders
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    // shaders are linked, we can delete intermediates
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}
int main()
{
    // ...
    std::string vertex_shader = ReadShaderFromFile("../shaders/vertex.glsl");
    std::string fragment_shader = ReadShaderFromFile("../shaders/fragment.glsl");
    unsigned int shader = CreateShaders(vertex_shader, fragment_shader);
    glUseProgram(shader);
    // ...
    glDeleteProgram(shader);
    return 0;    
}
```
we can also delete the source code using `glDetachShader` but we'll lose debugging capabilities and gain pretty much nothing.

### 3.2. basic vertex shader
the first line defines the version of glsl we're using. `core` will prevent use of any deprecated functions.<br>
`layout(location = 0)` indicates that the attribute we're gonna use, namely `in vec4 position` is at index 0. `gl_Position` is by default
a `vec4` object. opengl will automatically cast the `vec2`.
```cs
#version 330 core
layout(location = 0) in vec4 position;

void main()
{
    gl_Position = position;
}
```
#### 3.3. basic fragment shader
very similar to the vertex shader, only we specify the output color.
```cs
#version 330 core
layout(location = 0) out vec4 color;

void main()
{
    color = vec4(1.0, 0.0, 0.0, 1.0);
}
```

## 4. Drawing More Complex Objects

### 4.1 drawing a square
one way would be to draw two triangles. we'd need to change 3 parts
```cpp
float positions[] = {
        -0.5f,-0.5f,
         0.5f,-0.5f,
         0.5f, 0.5f,
         0.5f, 0.5f,
        -0.5f, 0.5f,
        -0.5f,-0.5f,
    };
glBufferData(GL_ARRAY_BUFFER, 6*2*sizeof(float), positions, GL_STATIC_DRAW);
glDrawArrays(GL_TRIANGLES, 0, 6);
```
this is however not efficient since we're reusings some vertices. we can use index buffers on top of the old buffer.
```cpp
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
// index buffer
unsigned int index_buffer_id;
glGenBuffers(1, &index_buffer_id);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW);
```
we don't save much space here by using `unsigned int` instead of e.g. `unsigned char`. later on with more complex model we have to use `unsigned int` anyway. we should also change the draw loop a bit
```cpp
while (!glfwWindowShouldClose(window))
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glfwSwapBuffers(window);
    glfwPollEvents();
}
```
**glDrawElements**
* GL_TRIANGLES : what we're drawing
* number of indices
* element type
* pointer to index buffer. since we have it bound, we can pass `nullptr`

## 5. OpenGL Error Handling
it's very easy to write an opengl program that compiles but doesn't produce a meaningful output.


### 5.1. polling
`glGetError` is a very old opengl feature, meaning it's also supported by almost all versions. after an error occurs, a flag is set which specifies the type of error. 

for example if we change `GL_UNSIGNED_INT` to `GL_INT` when calling `glDrawElements`, we won't get any errors and the program runs with a black screen. `glGetError` returns an enum each time it's called, until there are no other flags set, at which point it returns `GL_NO_ERROR = 0`.
```cpp
static void GLClearError()
{
    while(!glGetError());
}
bool GLLogCall(const char *function_name, const char* file, int line)
{
    if(GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error" << error << "] at " << function_name <<
           " in " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}
```
now we define some macros : 
```cpp
#define ASSERT(x) if (!(x)) __builtin_trap()
#define GLDebug(x) GLClearError();\
    x;\
    ASSERT(GLLogCal(#x, __FILE__, __LINE__))
```
wrapping any opengl function in GLDebug clears all the errors, calls the functions and asserts that it hasn't created any errors. it causes a `SIGILL` (illegal instruction) which can be caught by gdb. we can also use `raise(SIGTRAP)` which throws a breakpoint trap when executed.
```cpp
int main ()
{
    ...
    while (!glfwWindowShouldClose(window))
    {
        ...
        GLDebug(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr));
        ...
    }
}
```
output:
```
[OpenGL Error 1280] at glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr) in /home/hosein/Desktop/opengl/src/main.cpp:44
Illegal instruction (core dumped)
```
by looking up this code in `GL/glew.h`, we can see that the error is `GL_INVALID_ENUM`.
### 5.2. message callback
introdcued in opengl 4.3, it calls a user-defined callback function every time an error occurs. it also gives more info than the polling method.

## 6 Uniforms
uniforms are a way of moving data from cpu to shaders so it can be used as a variable. so far we've done it with vertex buffers.<br>
uniforms are set per each draw call whereas attributes are set per vertex. to send the uniform to the shader we have to make sure it's bound <br>
we want to send a vector of 4 float components. making a `glUniform` requires a location for the variable. once a shader is created, every uniforms
is assigned an id that is used to reference it. we typically get this id by looking up the variable by its name.
```cpp
...
glUseProgram(shader);
int location = glGetUniformLocation(shader, "u_color");
// wait for vsync
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
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform4f(location, r, 0.3f, 1.0f, 1.0f);
    ...
```
since opengl 4.3, it is also possible to explicitly set a uniform's location for the uniform inside the shader itself.<br>
we also have to define it in the shader
```cs
#version 330 core
layout(location = 0) out vec4 color;

uniform vec4 u_color;
void main()
{
    color = u_color;
}
```
## 7. Vertex Arrays
they are a way of binding a vertex buffer with a layout specification. right now we generate and bind our vertex buffer, specify its attributes and also bind our index buffer. if we want to draw two buffers with different layouts, we have to explicitly change layouts. the layout is not bound to the buffer.<br>
if we unbind everyhing using these three lines
```cpp
glBindBuffer(GL_ARRAY_BUFFER, 0);
glUseProgram(0);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
```
before drawing we would have to bind everything again and also specify the attributes (assuming we have multiple buffers with different layouts)
```cpp
glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (const void*)0);
glEnableVertexAttribArray(0);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
glUseProgram(shader);
glUniform4f(location, r, 0.3f, 1.0f, 1.0f);
```
a vertex array objects contains a binding between a vertex buffer (or buffers) and the attribute layout. our new approach will be
bind the shader, bind vertex array, bind index buffer, draw. vertex arrays are mandatory and a default one is created automatically 
if we pass `0` to `glEnableVertexAttribArray`, but only in compatibility profile. in core profile, they have to be explicitly defined.<br>
to run opengl in core profile, we have to add the following before creating the window
```cpp
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
```
### 7.1. creating a vertex array
a vertex array can be generated and bound before the buffer iteself. 
```cpp
unsigned int vao;
glGenVertexArrays(1, &vao);
glBindVertexArray(vao);
```
once created, we no longer need to bind the buffer and specify the layout for each object. it suffices to bind the vertex array and the index buffer. `glVertexAttribPointer` is the line that binds the vertex buffer to the vertex array.
```cpp
glBindVertexArray(vao);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
```
### experimental drivers
in linux it's very possible that you're using an experimental driver. experimental or pre-release drivers might not report every available extension through the standard mechanism, in which case GLEW will report it unsupported. to make sure all correct entry points are exposed, the global `glewExperimental` flag should be
set before initializing glew. 
```cpp
glewExperimental = GL_TRUE;
glewinit();
```
### 7.2. approaches
there are two general ways to handle VAOs.
#### global VAO
as mentioned, it is mandatory to create a vertex array in core profile. technically it is possible to create one VAO and leave it bound, bind a vertex buffer and
specify its layout, every time an object is to be drawn. this is the same thing the compatibility profile does for us.

#### seperate VAO
we could also create a vertex array object for every piece of geometry that we want to draw, bind its vertex buffer, define its attributes and then bind the VAO 
before every draw calls.

#### verdict
it **might** be faster to use a single global VAO (NVIDIA said so). they are however recommended by opengl. it's different on each environment/platform. needs some
benchmarking.

## 8. OOP Abstraction
