#ifndef _SHADER
#define _SHADER

#include "GlacierCore.h"

class GLACIER_API Shader
{
public:
	Shader(const std::string& file_name);
	Shader(const std::string& vertex_shader_file_name, const std::string& fragment_shader_file_name);
	Shader(const std::string& vertex_shader_file_name, const std::string& geometry_shader_file_name, const std::string& fragment_shader_file_name);
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader& operator=(Shader&&) = delete;
	~Shader();

	const GLuint& GetProgramID() const;

	void Bind() const;

	const GLint GetUniformLocation(const std::string& name) const;

	operator const GLuint& () const { return program_id; }

private:
	void load_shader(const char* const vertex_shader, const char* const fragment_shader);
	void load_with_geometry_shader(const char* const vertex_shader, const char* const fragment_shader, const char* const geometry_shader);

	GLuint program_id = 0;
};

#endif _SHADER