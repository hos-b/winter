#ifndef __WINTER_SHADER_H__
#define __WINTER_SHADER_H__

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>


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
	Shader(const std::string& vtx_path, const std::string& frag_path);
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
    int GetUniformLocation(const std::string& name) const;
	// single/separate shader files
	enum type
	{
		JOINT = 1,
		SEPERATE = 2
	}type_;
	std::vector<std::string> paths_;
	unsigned int renderer_id_;
	mutable std::unordered_map<std::string, int> uniform_cache_;
};

} // namespace base
} // namespace shader
#endif