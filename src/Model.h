#ifndef _MODEL
#define _MODEL

#include "VertexTypes.h"

enum class PREMADE_MODELS
{
	UNIT_CUBE_REPEAT_TEXTURE,
};

class Model
{
public:
	// Load a model from a GLTF file
	Model(const std::string& file_name);
	// Load a model from a given set of vertex and triangle data
	Model(const std::vector<VertexTypes::Vertex>& verts, const std::vector<VertexTypes::VertexTriangle>& triangles);
	// Load a model from a premade set
	Model(PREMADE_MODELS premade_model, const float& scale);
	// Load terrain from 16-bit heightmap
	Model(const std::string& file_name, const float& xz_size, const float& max_height, const float& u, const float& v);
	// Load a unit sphere model given a number of vertical and horizontal slices
	Model(const uint32_t& v_slices, const uint32_t& h_slices);
	// Load a flat plane given a xz size and uv repeats
	Model(const float& xz_size, const float& u, const float& v);

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
	Model(Model&& o) noexcept;
	Model& operator=(Model&& o);
	~Model();

	void Bind() const;

	const GLuint& GetVAO() const;
	const GLuint& GetVBO() const;
	const GLuint& GetEBO() const;
	const uint32_t& GetNumVertices() const;
	const uint32_t& GetNumTriangles() const;
	const uint32_t& GetNumBones() const;
	const std::vector<VertexTypes::Vertex>& GetVertexData() const;
	const std::vector<glm::mat4>& GetInverseBindMatrices() const;
	const std::vector<VertexTypes::VertexTriangle>& GetTriangles() const;

private:
	inline void calculate_tangents();

	void load_gpu_data();

	friend class ModelAtt;

private:
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	uint32_t num_vertices;
	uint32_t num_triangles;
	uint32_t num_bones;
	std::vector<VertexTypes::Vertex> vertex_data;
	std::vector<glm::mat4> inverse_bind_matrices;
	std::vector<VertexTypes::VertexTriangle> triangles;
};

#endif _MODEL