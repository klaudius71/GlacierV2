#ifndef _SHADER
#define _SHADER

class Shader
{
public:
	Shader(const std::string& file_name);
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader& operator=(Shader&&) = delete;
	~Shader();

	const GLuint& GetProgramID() const;

	void Bind() const;

	const GLint GetUniformLocation(const std::string& name) const;

private:
	void load_shader(const char* const vertex_shader, const char* const fragment_shader);
	void load_with_geometry_shader(const char* const vertex_shader, const char* const fragment_shader, const char* const geometry_shader);

	GLuint program_id = 0;
};

#endif _SHADER