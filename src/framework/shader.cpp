#include "framework/shader.h"
#include "framework/renderer.h"

#include <iostream>
#include <sstream>
#include <fstream>

Shader::Shader(const std::string& file_path) : file_path_(file_path)
{
    ShaderProgramSource source = ParseShader();
    renderer_id_ = CreateShaders(source.vertex_source, source.fragment_source);
}
Shader::~Shader()
{
    GLDebug(glDeleteProgram(renderer_id_));
}
void Shader::Bind() const
{
    GLDebug(glUseProgram(renderer_id_));
}
void Shader::Unbind() const
{
    GLDebug(glUseProgram(0));
}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // = &source[0]
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char*) alloca(length*sizeof(char)); // dynamic stack memory
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile " << (type==GL_VERTEX_SHADER ? "vertex shader":"fragment shader") << std::endl;
        std::cout << message;
        glDeleteShader(id);
        return -1;
    }
    return id;
}

unsigned int Shader::CreateShaders(const std::string& vertex_shader, const std::string& fragment_shader)
{
    GLDebug(unsigned int program = glCreateProgram());
    // compiling shaders
    unsigned int vs = CompileShader(vertex_shader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragment_shader, GL_FRAGMENT_SHADER);
    // linking shaders
    GLDebug(glAttachShader(program, vs));
    GLDebug(glAttachShader(program, fs));
    GLDebug(glLinkProgram(program));
    GLDebug(glValidateProgram(program));
    // shaders are attached now, we can delete intermediates
    GLDebug(glDeleteShader(vs));
    GLDebug(glDeleteShader(fs));
    return program;
}

ShaderProgramSource Shader::ParseShader()
{
    std::ifstream stream(file_path_);
    enum class ShaderType
    {
        NONE=-1, VERTEX=0, FRAGMENT=1
    };
    std::string line;
    std::stringstream stringstream[2];
    ShaderType type = ShaderType::NONE;
    while(std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
            stringstream[(int)type] << line << "\n";
    }
    return { stringstream[0].str(), stringstream[1].str() };
}

// set uniforms
void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    GLDebug(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
void Shader::SetUniform1i(const std::string &name, int v0)
{
    GLDebug(glUniform1i(GetUniformLocation(name), v0));
}
void Shader::SetUniform1f(const std::string &name, float v0)
{
    GLDebug(glUniform1f(GetUniformLocation(name), v0));
}
void Shader::SetUniform4x4f(const std::string &name, glm::mat4& mat)
{
    GLDebug(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}
int Shader::GetUniformLocation(const std::string& name) const
{
    auto search_result = uniform_cache_.find(name);
    if (search_result != uniform_cache_.end())
        return search_result->second;
    GLDebug(int location = glGetUniformLocation(renderer_id_, name.c_str()));
    if (location == -1)
        std::cout << "warning : uniform " << name << " doesn't exist" << std::endl;
    uniform_cache_[name] = location;
    return location;
}
