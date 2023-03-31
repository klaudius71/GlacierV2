#ifndef _SHADER_OPENGL
#define _SHADER_OPENGL

#include "Shader.h"

class GLACIER_API ShaderOpenGL : public Shader
{
public:
	ShaderOpenGL(const std::string& file_name);
	ShaderOpenGL(const std::string& vertex_shader_file_name, const std::string& fragment_shader_file_name);
	ShaderOpenGL(const std::string& vertex_shader_file_name, const std::string& geometry_shader_file_name, const std::string& fragment_shader_file_name);
	ShaderOpenGL(const ShaderOpenGL&) = delete;
	ShaderOpenGL& operator=(const ShaderOpenGL&) = delete;
	ShaderOpenGL(ShaderOpenGL&&) = delete;
	ShaderOpenGL& operator=(ShaderOpenGL&&) = delete;
	~ShaderOpenGL();

	GLuint GetProgramID() const;

	virtual void Bind() const override;

	const GLint GetUniformLocation(const std::string& name) const;

	operator const GLuint& () const { return program_id; }

private:
	void load_shader(const char* const vertex_shader, const char* const fragment_shader);
	void load_shader(const char* const vertex_shader, const char* const geometry_shader, const char* const fragment_shader);
	void load_uniforms();

	GLuint program_id = 0;
	std::unordered_map<std::string, GLuint> uniform_locations;
};

#endif