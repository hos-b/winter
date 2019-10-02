#ifndef __GL_SHADER_H__
#define __GL_SHADER_H__

#include <string>
#include <iostream>
#include <unordered_map>

struct ShaderProgramSource
{
    std::string vertex_source;
    std::string fragment_source;
};

class Shader
{
public:
    Shader(const std::string& filename);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    unsigned int CompileShader(const std::string& source, unsigned int type);
    ShaderProgramSource ParseShader();
    unsigned int CreateShaders(const std::string& vertex_shader, const std::string& fragment_shader);

    // set uniforms
    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
    void SetUniform1f(const std::string &name, float v0);
    void SetUniform1i(const std::string &name, int v0);
    
private:
    unsigned int renderer_id_;
    std::string file_path_;
    int GetUniformLocation(const std::string& name);
    std::unordered_map<std::string, int> uniform_cache_;
};

#endif
