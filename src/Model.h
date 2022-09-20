#ifndef _MODEL
#define _MODEL

#include "VertexTypes.h"

enum class PREMADE_MODELS
{
	UNIT_CUBE_REPEAT_TEXTURE,
	UNIT_CUBE_WRAP_TEXTURE
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
	Model(const int& v_slices, const int& h_slices);
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
	const std::vector<VertexTypes::Vertex>& GetVertexData() const;
	const std::vector<glm::vec3>& GetVertices() const;
	const std::vector<glm::vec2>& GetUVs() const;
	const std::vector<glm::vec3>& GetNormals() const;
	const std::vector<glm::vec3>& GetTangents() const;
	const std::vector<glm::vec3>& GetBitangents() const;
	const std::vector<glm::uvec4>& GetJointIDs() const;
	const std::vector<glm::vec4>& GetJointWeights() const;
	const std::vector<VertexTypes::VertexTriangle>& GetTriangles() const;

private:
	inline void calculate_tangents();
	void populate_all_arrays_from_vertex_data();

	static void load_GPU_data(Model& mod);

	friend class ModelLoader;

private:
	GLuint vao = 0xFFFFFFFF;
	GLuint vbo = 0xFFFFFFFF;
	GLuint ebo = 0xFFFFFFFF;
	uint32_t num_vertices;
	uint32_t num_triangles;
	std::vector<VertexTypes::Vertex> vertex_data;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	std::vector<glm::uvec4> joint_ids;
	std::vector<glm::vec4> joint_weights;
	std::vector<VertexTypes::VertexTriangle> triangles;
};

#endif _MODEL