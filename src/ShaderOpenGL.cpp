#include "gpch.h"
#include "ShaderOpenGL.h"

ShaderOpenGL::ShaderOpenGL(const std::string& file_name)
{
	std::ifstream file(file_name + ".vs.glsl", std::ios::in);
	assert(file.is_open() && "File not found!");
	std::stringstream strm;
	strm << file.rdbuf();
	std::string vert(strm.str());
	strm.str("");
	file.close();
	file.clear();

	file.open(file_name + ".fs.glsl", std::ios::in);
	assert(file.is_open() && "File not found!");
	strm << file.rdbuf();
	std::string frag(strm.str());
	strm.str("");
	file.close();
	file.clear();

	file.open(file_name + ".gs.glsl", std::ios::in);
	if (file.is_open())
	{
		strm << file.rdbuf();
		std::string geo(std::move(strm.str()));
		load_shader(vert.c_str(), geo.c_str(), frag.c_str());
	}
	else
	{
		load_shader(vert.c_str(), frag.c_str());
	}
	file.close();
}
ShaderOpenGL::ShaderOpenGL(const std::string& vertex_shader_file_name, const std::string& fragment_shader_file_name)
{
	std::ifstream file(vertex_shader_file_name, std::ios::in);
	assert(file.is_open() && "File not found!");
	std::stringstream strm;
	strm << file.rdbuf();
	std::string vert(strm.str());
	strm.str("");
	file.close();
	file.clear();

	file.open(fragment_shader_file_name, std::ios::in);
	assert(file.is_open() && "File not found!");
	strm << file.rdbuf();
	std::string frag(strm.str());
	strm.str("");
	file.close();
	file.clear();

	load_shader(vert.c_str(), frag.c_str());
}
ShaderOpenGL::ShaderOpenGL(const std::string& vertex_shader_file_name, const std::string& geometry_shader_file_name, const std::string& fragment_shader_file_name)
{
	std::ifstream file(vertex_shader_file_name, std::ios::in);
	assert(file.is_open() && "File not found!");
	std::stringstream strm;
	strm << file.rdbuf();
	std::string vert(strm.str());
	strm.str("");
	file.close();
	file.clear();

	file.open(geometry_shader_file_name, std::ios::in);
	assert(file.is_open() && "File not found!");
	strm << file.rdbuf();
	std::string geo(strm.str());
	strm.str("");
	file.close();
	file.clear();

	file.open(fragment_shader_file_name, std::ios::in);
	assert(file.is_open() && "File not found!");
	strm << file.rdbuf();
	std::string frag(strm.str());
	strm.str("");
	file.close();
	file.clear();

	load_shader(vert.c_str(), geo.c_str(), frag.c_str());
}
ShaderOpenGL::~ShaderOpenGL()
{
	glDeleteProgram(program_id);
}

GLuint ShaderOpenGL::GetProgramID() const
{
	return program_id;
}

void ShaderOpenGL::Bind() const
{
	glUseProgram(program_id);
}

const GLint ShaderOpenGL::GetUniformLocation(const std::string& name) const
{
	auto it = uniform_locations.find(name);
	assert(it != uniform_locations.cend());
	return it->second;
}

void ShaderOpenGL::load_shader(const char* const vertex_shader, const char* const fragment_shader)
{
	GLuint vert_shad = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shad, 1, &vertex_shader, nullptr);
	glCompileShader(vert_shad);

	GLsizei length;
	GLchar log[512];
	glGetShaderInfoLog(vert_shad, 512, &length, log);
	if (length != 0)
	{
		OutputDebugString(log);
		assert(false);
	}

	GLuint frag_shad = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shad, 1, &fragment_shader, nullptr);
	glCompileShader(frag_shad);
	glGetShaderInfoLog(frag_shad, 512, &length, log);
	if (length != 0)
	{
		OutputDebugString(log);
		assert(false);
	}

	program_id = glCreateProgram();
	glAttachShader(program_id, vert_shad);
	glAttachShader(program_id, frag_shad);

	glLinkProgram(program_id);

	glDeleteShader(vert_shad);
	glDeleteShader(frag_shad);

	load_uniforms();
}
void ShaderOpenGL::load_shader(const char* const vertex_shader, const char* const geometry_shader, const char* const fragment_shader)
{
	GLuint vert_shad = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shad, 1, &vertex_shader, nullptr);
	glCompileShader(vert_shad);

	GLsizei length;
	GLchar log[512];
	glGetShaderInfoLog(vert_shad, 512, &length, log);
	if (length != 0)
	{
		printf("%s", log);
		assert(false);
	}

	GLuint frag_shad = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shad, 1, &fragment_shader, nullptr);
	glCompileShader(frag_shad);
	glGetShaderInfoLog(vert_shad, 512, &length, log);
	if (length != 0)
	{
		printf("%s", log);
		assert(false);
	}

	GLuint geo_shad = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geo_shad, 1, &geometry_shader, nullptr);
	glCompileShader(geo_shad);
	glGetShaderInfoLog(vert_shad, 512, &length, log);
	if (length != 0)
	{
		printf("%s", log);
		assert(false);
	}

	GLuint shad = glCreateProgram();
	glAttachShader(shad, vert_shad);
	glAttachShader(shad, frag_shad);
	glAttachShader(shad, geo_shad);

	glLinkProgram(shad);
	glUseProgram(shad);

	glDeleteShader(vert_shad);
	glDeleteShader(frag_shad);
	glDeleteShader(geo_shad);

	load_uniforms();
}
void ShaderOpenGL::load_uniforms()
{
	int count;
	glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &count);

	GLsizei length;
	GLint size;
	GLenum type;
	GLchar name[64];
	GLsizei bufSize = 64;
	for (int i = 0; i < count; i++)
	{
		glGetActiveUniform(program_id, (GLuint)i, bufSize, &length, &size, &type, name);
		uniform_locations.emplace(name, glGetUniformLocation(program_id, name));
	}
}