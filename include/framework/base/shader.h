#ifndef __GL_SHADER_H__
#define __GL_SHADER_H__

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>


namespace winter
{
namespace base
{

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

    // uniform template
    template<class T, int N, class... Ts>
    void SetUniform(const std::string& name, Ts... args);
    
private:
    unsigned int renderer_id_;
    std::string file_path_;
    int GetUniformLocation(const std::string& name) const;
    mutable std::unordered_map<std::string, int> uniform_cache_;
};

}// end of base
}// end of shader
#endif
