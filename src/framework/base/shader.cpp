#include "framework/base/shader.h"
#include "framework/misc/debug.h"


#include <GL/glew.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
namespace winter
{
namespace base
{
    
Shader::Shader(const std::string& file_path)
{
	type_ = Shader::JOINT;
	paths_.emplace_back(file_path);
	ShaderProgramSource source = ParseShader();
	renderer_id_ = CreateShaders(source.vertex_source, source.fragment_source, "");
}
Shader::Shader(const std::string& vtx_path, const std::string& frag_path)
{
	type_ = Shader::SEPERATE;
	paths_.emplace_back(vtx_path);
	paths_.emplace_back(frag_path);
	ShaderProgramSource source = ParseShader();
	renderer_id_ = CreateShaders(source.vertex_source, source.fragment_source, "");
}
Shader::Shader(const std::string& vtx_path, const std::string& frag_path, const std::string& geom_path)
{
	type_ = Shader::SEPERATE;
	paths_.emplace_back(vtx_path);
	paths_.emplace_back(frag_path);
	paths_.emplace_back(geom_path);
	ShaderProgramSource source = ParseShader();
	renderer_id_ = CreateShaders(source.vertex_source, source.fragment_source, source.geometry_source);
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
        std::cout << "failed to compile " << (type==GL_VERTEX_SHADER ? "vertex shader": 
											  type==GL_FRAGMENT_SHADER ? "fragment shader":
											  "geometry_shader") << std::endl;
        std::cout << message;
        glDeleteShader(id);
		throw "shader";
		return -1;
	}
    return id;
}

unsigned int Shader::CreateShaders(const std::string& vertex_shader, const std::string& fragment_shader, const std::string& geometry_shader)
{
    GLDebug(unsigned int program = glCreateProgram());
    // compiling shaders
    unsigned int vs = CompileShader(vertex_shader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragment_shader, GL_FRAGMENT_SHADER);
	unsigned int gs = 0;
	if (geometry_shader.length() > 0)
		gs = CompileShader(geometry_shader, GL_GEOMETRY_SHADER);
	// linking shaders
	GLDebug(glAttachShader(program, vs));
    GLDebug(glAttachShader(program, fs));
	if (geometry_shader.length() > 0)
		GLDebug(glAttachShader(program, gs));
    GLDebug(glLinkProgram(program));
    GLDebug(glValidateProgram(program));
	// shaders are attached now, we can delete intermediates
	GLDebug(glDeleteShader(vs));
    GLDebug(glDeleteShader(fs));
	if (geometry_shader.length() > 0)
		GLDebug(glDeleteShader(gs));
    return program;
}

ShaderProgramSource Shader::ParseShader()
{
	std::stringstream stringstream[3];
	if(type_==Shader::SEPERATE)
	{
		std::ifstream vtx(paths_[0]), frag(paths_[1]);
		stringstream[0] << vtx.rdbuf();
		stringstream[1] << frag.rdbuf();
		// if geometry shader present
		if(paths_.size() == 3)
		{
			std::ifstream geom(paths_[2]);
			stringstream[2] << geom.rdbuf();
		}
	}
	else
	{
		std::ifstream stream(paths_[0]);
		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
		};
		std::string line;
    	ShaderType type = ShaderType::NONE;
    	while(std::getline(stream, line))
    	{
    	    if (line.find("#shader") != std::string::npos)
    	    {
    	        if (line.find("vertex") != std::string::npos)
    	            type = ShaderType::VERTEX;
    	        else if (line.find("fragment") != std::string::npos)
    	            type = ShaderType::FRAGMENT;
				else if (line.find("geometry") != std::string::npos)
    	            type = ShaderType::GEOMETRY;
    	    }
    	    else
    	        stringstream[(int)type] << line << "\n";
    	}	
	}
    return { stringstream[0].str(), stringstream[1].str(), stringstream[2].str() };
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
// uniforms -------------------------------------------------------------------------
template<>
void Shader::SetUniform<int, 1>(const std::string &name, int arg)
{
    GLDebug(glUniform1i(GetUniformLocation(name), arg));
}
template<>
void Shader::SetUniform<int, 1>(const std::string &name, unsigned int arg)
{
    GLDebug(glUniform1i(GetUniformLocation(name), arg));
}
template<>
void Shader::SetUniform<float, 1>(const std::string& name, float arg)
{
    GLDebug(glUniform1f(GetUniformLocation(name), arg));
}
template<>
void Shader::SetUniform<float, 3>(const std::string &name, float arg0, float arg1, float arg2)
{
    GLDebug(glUniform3f(GetUniformLocation(name), arg0, arg1, arg2));
}
template<>
void Shader::SetUniform<glm::vec3, 3>(const std::string &name, glm::vec3 arg)
{
    GLDebug(glUniform3f(GetUniformLocation(name), arg.x, arg.y, arg.z));
}
template<>
void Shader::SetUniform<float, 4>(const std::string &name, float arg0, float arg1, float arg2, float arg3)
{
    GLDebug(glUniform4f(GetUniformLocation(name), arg0, arg1, arg2, arg3));
}
template<>
void Shader::SetUniform<float*, 16>(const std::string &name, float *arg)
{
    GLDebug(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, arg));
}
template<>
void Shader::SetUniform<glm::mat4, 16>(const std::string &name, glm::mat4 arg)
{
    GLDebug(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &arg[0][0]));
}

} // namespace base
} // namespace shader