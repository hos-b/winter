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
## 2. Vertex Buffers and Atttribute Layouts
we want to draw a triangle

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
it doesn't matter where we enable the attribute as long as the buffer is bound

## 3. Shaders
the code might already be able to draw a triangle. this is because some drivers already provide a default shader. shaders are programmable parts of the rendering
pipeline. we have 5 shaders in opengl but normally only the first two are used.
* vertex shader
* fragment shader
* geometry shader
* tesselation shader
* compute shader

#### vertex shader
the primary purpose of the vertex shader is to define where in the window the vertices are drawn. it gets called once for every vertex that we have. it's not optional and must store something in `gl_Position` as it is used by later stages. it can also specify additional outputs that can be used by later user-defined shaders. input is the vertex data itself.
#### fragment shader
the fragment shader is called once per every pixel that get rasterized and determines the color of that pixel. the coordinates of each of these pixels is obtained by interpolating between the given vertices. this leads to a very accurate estimation of where the pixel should be. this means the fragment shader gets called way more often and has to be optimized. it's optional but very often used. most important output is a `vec4` color for each pixel.s

#### rendering pipleline
* vertex specification
* vertex shader
* tesselation shader
* geometry shader
* vertex post-processing
* primitive assembly
* rasterization
* fragment shader
* per-sample operations

tesselation shader allows us to divide data into smaller primitives. appeared in version 4.0, can be used to add higher levels of detail **dynamically**. <br>
geometry shader works on groups of vertices or primitives. it's used to alter primitives.<br>
vertex post processing consists of transform feedback and clipping. transform feedback, if enabled, saves the result of vertex and geometry shaders in a buffer for later use. clipping removes non-visible primitives. also converts positions from clip space to window space.<br>
primitive assembly takes care of converting vertices into a series of primitives. it also performs face culling <br>
rasterization converts primitives into fragments, i.e. pixel data. fragments are interpolated based on vertex positions<br>
per sample operatios is a series of test to see if should must be drawn. most importantly depth test. it also takes care of blending
### 3.1. compiling shaders
1. create empty program
2. create empty shaders
3. attach shader source code to shaders
4. compile shaders
5. attach them to the program
6. link program
7. validate program

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
#### Note
make sure the shader is bound before using `glUniform`!
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
### 7.3. approaches
there are two general ways to handle VAOs.
#### global VAO
as mentioned, it is mandatory to create a vertex array in core profile. technically it is possible to create one VAO and leave it bound, bind a vertex buffer and
specify its layout, every time an object is to be drawn. this is the same thing the compatibility profile does for us.

#### seperate VAO
vertex specification is done as such:
1. generate a vertex array object, get the id
2. bind it using the id
3. generate a vertex buffer, get the id
4. bind the id
5. attach vertex data to that buffer
6. define attribute pointer formatting
7. enable it
8. unbind VAO and VBO, do it for next object
and then we draw:
1. activate shader
2. bind VAO of the object to be drawn
3. call `glDrawArrays`

#### verdict
it **might** be faster to use a single global VAO (NVIDIA said so). they are however recommended by opengl. it's different on each environment/platform. needs some
benchmarking.

#### OOP
at this point we can make abstractions for index buffers, vertex buffers, vertex arrays, shaders and renderer.

## 8. Textures
an image we can use in rendering, which is sent to the GPU and is accessible in the shader.

### 8.1. creating a texture
we use stb_image to handle loading of textures into RAM. opengl expects the texture to start at the bottom left, not top left. this is why we flip the images 
vertically on load.

```cpp
glGenTextures(1, &renderer_id_);
glBindTexture(GL_TEXTURE_2D, renderer_id_);

stbi_set_flip_vertically_on_load(1);
local_buffer_ = stbi_load(file_path_.c_str(), &width_, &height_, &bits_per_pixel_, 4);
```
we then set some of the parameters provided by the opengl api. `MIN` and `MAG` filters specify what opengl does when the texture is larger bzw. smaller than the
area it's mapped on to. another option would be `GL_NEAREST`.
```cpp    
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer_);
```
we then unbind the texture and free the local buffer. sometimes it might be necessary to keep it in case we want to sample from it.
```cpp
glBindTexture(GL_TEXTURE_2D, 0);
if(local_buffer_)
    stbi_image_free(local_buffer_);
```

### 8.2. binding
in opengl we have up to 32 texture slots for each texture enum. it's platform dependent how many we get. here we actiave the first texture and bind it.
```cpp
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, renderer_id_);
```

### 8.2. shader code
we have to send a uniform (1i) to our shader to specift which slot we're accessing.
```cpp
shader.SetUniform1i("u_texture", 0);
```
#### texture coordinates
using texture coordinates, we specify which part of the image each vertex represents. in the fragment shader we interpolate between these coordinates to get the
correct color for that pixel. 

we extend the vertex buffer to now contain vertex coordinates and their respective texture coordinates. we also increase the size of our vertex buffer and add the
new layout
```cpp
float positions[] = {
   -0.5f,-0.5f, 0.0f, 0.0f,  // 0
    0.5f,-0.5f, 1.0f, 0.0f,  // 1
    0.5f, 0.5f, 1.0f, 1.0f,  // 2
   -0.5f, 0.5f, 0.0f, 1.0f   // 3
};
VertexBuffer vb(positions, 4 * 4 *sizeof(float));
VertexBufferLayout layout;
layout.Push<float>(2);
layout.Push<float>(2);
va.AddBuffer(vb, layout);
```
in the vertex shader we get the texture coordinates and also mark them as output to send them to the fragment shader. in the fragment shader we first get the 
texture coordinates from the vertex shader and then use a `sampler2D` to sample from the texture based on the coordinates.
```cs
#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coord;
out vec2 v_texcoord;
void main()
{
    gl_Position = position;
    v_texcoord = tex_coord;
};
#shader fragment
#version 330 core
layout(location = 0) out vec4 color;
in vec2 v_texcoord
uniform vec4 u_color;
uniform sampler2D u_texture;
void main()
{
    vec4 tex_color = texture(u_texture, v_texcoord); 
    color = tex_color;
};
```
### 8.3. blending
we should also enable blending somewhere to get the alpha channel working
```cpp
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
glEnable(GL_BLEND));
```
this decides how the output of our fragment shader is mixed with our draw buffer. the parameters define how the RGBA values are computed for source and destionation
respectively. by default the values are `GL_ONE` & `GL_ZERO` <br>
we can also specify a blend function.
```cpp
glBlendEquation(GL_FUNC_ADD);
```
### 8.4. repeat
another way to map textures onto surfaces is to repeat them. this is specified when we create the shaders.
```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
```
we have to specify in our vertex buffer how many times we want them to repeat by adjusting their respective s,t values
```cpp
float positions[] = {
        -0.5f,-0.5f, 0.0f, 0.0f,  // 0
         0.5f,-0.5f, 2.0f, 0.0f,  // 1
         0.5f, 0.5f, 2.0f, 2.0f,  // 2
        -0.5f, 0.5f, 0.0f, 2.0f   // 3
    };
```
this will repeat them twice in each direction

## 9. Mathematics Library
[glm](https://github.com/g-truc/glm) is an open source header-only library specifically made for use with opengl. e.g. all matrix implementations are column major.

### 9.1. 2D projection matrix
right now our window has a 4:3 aspect ration but opengl uses a 1:1 by default. we can change that using glm::ortho
```cpp
glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
```
where the arguements are left, right, bottom, top, zNear, zFar. having this projection matrix changes how are vertex positions are interpreted. now they have to be
in the same range we defined in the orthographic matrix but in the end opengl only draws the vertices in the (-1, 1) frustum and culls the rest. to correctly remap the values back to this range, we multiply our vertices with this matrix in the shader.
```cs
...
uniform mat4 u_model_view_projection;
void main()
{
    gl_Position = u_model_view_projection * position;
    ...
};
```
we also have to send the uniform to the shader. the necessary implementation should be created in the shader class. since it doesn't need to be transposed, we pass
`GL_FALSE` for the third arguement.
```cpp
void Shader::SetUniform4x4f(const std::string &name, glm::mat4& mat)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}
```
to make things a bit easier, we can move to pixel space, i.e. set the projection matrix such that the minimum and maximum values defines our windows size. this way
we'd also have to change how our vertices are positioned
```cpp
glm::mat4 projection = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
float positions[] = {
        100.0f, 100.0f, 0.0f, 0.0f,  // 0
        300.0f, 100.0f, 1.0f, 0.0f,  // 1
        300.0f, 300.0f, 1.0f, 1.0f,  // 2
        100.0f, 300.0f, 0.0f, 1.0f   // 3
    };
```
### 9.2. view matrix
2D position = projection x view x model x vertex<br>
the view matrix defines the transform of our camera. the opengl camera is on the z axis, placed at 1, looking in the negative z direction. we can't actually move the
camera but we can move everything else. e.g. instead of moving our camera to the right, we move our objects to the left.
```cpp
glm::mat4 view =  glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
glm::mat4 mvp = projection*view;
shader.SetUniform4x4f("u_model_view_projection", mvp);
```

### 9.3. model matrix
the model matrix defines it for the model itself. this is more intuitive than adjusting the view matrix. we start off with an identity matrix and translate it
```cpp
...
glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(300, 300, 0));
glm::mat4 mvp = projection*view*model;
shader.SetUniform4x4f("u_model_view_projection", mvp);
```

## 10. imGui
"Dear [ImGui](https://github.com/ocornut/imgui) is a bloat-free graphical user interface library for C++. It outputs optimized vertex buffers that you can render anytime in your 3D-pipeline enabled application. It is fast, portable, renderer agnostic and self-contained (no external dependencies)." nice ...

### 10.1. installation
get the code (currenly using release version 1.73), remove everything other than base h/cpp files, add every opengl3/glfw related h/cpp file in examples other than
main.cpp. keep that for reference.

### 10.2. basic example
using the provided example from imgui, we mix is with our code
```cpp
GLFWwindow* window = ...;
//imgui
const char* glsl_version = "#version 330";
ImGui::CreateContext();
ImGuiIO io = ImGui::GetIO(); (void)io;
ImGui::StyleColorsDark();
ImGui_ImplGlfw_InitForOpenGL(window, true);
ImGui_ImplOpenGL3_Init(glsl_version);
```
for the rendering loop we have
```cpp
while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        // start the imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // opengl code
        ...

        // render imgui stuff
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
```
and before killing the glfw context, we clean up
```cpp
ImGui_ImplOpenGL3_Shutdown();
ImGui_ImplGlfw_Shutdown();
ImGui::DestroyContext(NULL);
```
## 11. 