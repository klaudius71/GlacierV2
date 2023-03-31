#include "gpch.h"
#include "ModelOpenGL.h"

ModelOpenGL::ModelOpenGL(ModelOpenGL&& o) noexcept
	: Model(std::move(o)), vao(o.vao), vbo(o.vbo), ebo(o.ebo)
{
	o.vao = 0;
	o.vbo = 0;
	o.ebo = 0;
}
ModelOpenGL& ModelOpenGL::operator=(ModelOpenGL&& o)
{
	Model::operator=(std::move(o));
	vao = o.vao;
	vbo = o.vbo;
	ebo = o.ebo;

	o.vao = 0;
	o.vbo = 0;
	o.ebo = 0;
	return *this;
}
ModelOpenGL::~ModelOpenGL()
{
	// By default OpenGL silently ignores buffer 0, but this destructor is called in a separate thread so I can't call gl functions at all
	if (vao != 0)
	{
		glDeleteBuffers(2, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
}

void ModelOpenGL::Bind() const
{
	glBindVertexArray(vao);
}

const GLuint ModelOpenGL::GetVAO() const
{
	return vao;
}
const GLuint ModelOpenGL::GetVBO() const
{
	return vbo;
}
const GLuint ModelOpenGL::GetEBO() const
{
	return ebo;
}

void ModelOpenGL::load_gpu_data()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexTypes::Vertex) * num_vertices, vertex_data.data(), GL_STATIC_DRAW);

	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTypes::Vertex), (void*)0);

	// UVs
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTypes::Vertex), (void*)(sizeof(glm::vec3)));

	// Tex_ID
	glEnableVertexAttribArray(2);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(VertexTypes::Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	// Normal
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTypes::Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(uint32_t)));

	// Tangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTypes::Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(uint32_t) + sizeof(glm::vec3)));

	// Bitangent
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTypes::Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(uint32_t) + sizeof(glm::vec3) * 2));

	// Joint IDs
	glEnableVertexAttribArray(6);
	glVertexAttribIPointer(6, 4, GL_UNSIGNED_INT, sizeof(VertexTypes::Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(uint32_t) + sizeof(glm::vec3) * 3));

	// Joint Weights
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(VertexTypes::Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(uint32_t) + sizeof(glm::vec3) * 3 + sizeof(glm::uvec4)));

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(VertexTypes::VertexTriangle) * GetNumTriangles(), triangles.data(), GL_STATIC_DRAW);
}