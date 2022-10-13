#include "gpch.h"
#include "Model.h"
#include "VertexTypes.h"
#include "stb_image.h"

Model::Model(const std::string& file_name)
{
	std::ifstream file(file_name);
	assert(file.is_open());
	const nlohmann::json j = std::move(nlohmann::json::parse(file, nullptr, false, true));
	file.close();

	const std::string& uri = j["buffers"][0]["uri"];
	const std::vector<uint8_t> buffer_data = base64_decode_bytes(uri.substr(uri.find(',') + 1));
	assert(buffer_data.size() == j["buffers"][0]["byteLength"]);

	struct BufferData
	{
		uint32_t stride;
		uint32_t count;
		const void* data;

		BufferData(uint32_t stride, uint32_t count, const void* data)
			: stride(stride), count(count), data(data) {}
	};
	std::map<std::string, BufferData> data;

	assert(j.find("meshes") != j.cend());
	const char* atts[] = { "POSITION", "NORMAL", "TEXCOORD_0", "JOINTS_0", "WEIGHTS_0" };
	const auto& primitives = j["meshes"][0]["primitives"][0];
	const auto& attributes = primitives["attributes"];
	for (auto att : atts)
	{
		auto it = attributes.find(att);
		if (it != attributes.end())
		{
			const uint32_t accessor_index = it.value();
			const auto& accessor = j["accessors"][accessor_index];
			const auto& bufferView = j["bufferViews"][accessor_index];
			const uint8_t* buff = buffer_data.data() + (uint32_t)bufferView["byteOffset"];
			const std::string type = accessor["type"];
			const uint32_t count = accessor["count"];
			if (type == "VEC3")
				data.emplace(std::piecewise_construct, std::forward_as_tuple(att), std::forward_as_tuple(4, count * 3, buff));
			else if (type == "VEC2")
				data.emplace(std::piecewise_construct, std::forward_as_tuple(att), std::forward_as_tuple(4, count * 2, buff));
			else if (type == "VEC4")
				data.emplace(std::piecewise_construct, std::forward_as_tuple(att), std::forward_as_tuple(4, count * 4, buff));

			num_vertices = count;
		}
	}

	{
		const auto& indices = primitives.find("indices");
		uint32_t accessor_index = indices.value();
		const auto& accessor = j["accessors"][accessor_index];
		const auto& bufferView = j["bufferViews"][accessor_index];
		const uint8_t* buff = buffer_data.data() + (uint32_t)bufferView["byteOffset"];
		const auto& componentType = accessor["componentType"];
		const uint32_t count = accessor["count"];
		if (componentType == GL_UNSIGNED_SHORT)
			data.emplace(std::piecewise_construct, std::forward_as_tuple("indices"), std::forward_as_tuple(2, count, buff));
		else if (componentType == GL_UNSIGNED_INT)
			data.emplace(std::piecewise_construct, std::forward_as_tuple("indices"), std::forward_as_tuple(4, count, buff));
	}

	assert(data.find("POSITION") != data.cend());

	const glm::vec3* pos = (glm::vec3*)data.find("POSITION")->second.data;
	const glm::vec2* uv = nullptr;
	if (data.find("TEXCOORD_0") != data.end())
		uv = (glm::vec2*)data.find("TEXCOORD_0")->second.data;
	const glm::vec3* norm = nullptr;
	if (data.find("NORMAL") != data.end())
		norm = (glm::vec3*)data.find("NORMAL")->second.data;

	vertices = std::vector<glm::vec3>(pos, pos + num_vertices);
	uvs = std::vector<glm::vec2>(uv, uv + num_vertices);
	normals = std::vector<glm::vec3>(norm, norm + num_vertices);

	const auto& ind = data.find("indices")->second;
	num_triangles = ind.count / 3;
	if (ind.stride == 2)
	{
		const uint16_t* const indices = (const uint16_t*)ind.data;
		for (size_t i = 0; i < num_triangles; i++)
			triangles.emplace_back(indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2]);
	}
	else if (ind.stride == 4)
	{
		const uint32_t* const indices = (const uint32_t*)ind.data;
		for (size_t i = 0; i < num_triangles; i++)
			triangles.emplace_back(indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2]);
	}

	std::vector<VertexTypes::Vertex> verts;
	verts.reserve(num_vertices);
	
	const auto& joint_ids_data = data.find("JOINTS_0");
	if (joint_ids_data != data.end())
	{
		assert(data.find("WEIGHTS_0") != data.end());
		const uint8_t* const joint = (const uint8_t*)joint_ids_data->second.data;
		joint_ids.reserve(num_vertices);
		for (uint32_t i = 0; i < num_vertices; i++)
			joint_ids.emplace_back(joint[i * 4], joint[i * 4 + 1], joint[i * 4 + 2], joint[i * 4 + 3]);
		
		const glm::vec4* const weights = (const glm::vec4*)data.find("WEIGHTS_0")->second.data;
		joint_weights = std::vector<glm::vec4>(weights, weights + num_vertices);

		for (uint32_t i = 0; i < num_vertices; i++)
			verts.emplace_back(vertices[i], uvs[i], 0, normals[i], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,0.0f,0.0f), joint_ids[i], joint_weights[i]);
	}
	else
	{
		for (uint32_t i = 0; i < num_vertices; i++)
			verts.emplace_back(vertices[i], uvs[i], 0, normals[i], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::uvec4(0,0,0,0), glm::vec4(1.0f,1.0f,1.0f,1.0f));
	}
}
Model::Model(const std::vector<VertexTypes::Vertex>& verts, const std::vector<VertexTypes::VertexTriangle>& triangles)
	: vertex_data(verts), triangles(triangles), num_vertices((uint32_t)verts.size()), num_triangles((uint32_t)triangles.size())
{
	assert(triangles.size() > 0 && verts.size() > 0);
	populate_all_arrays_from_vertex_data();
	load_GPU_data(*this);
}
Model::Model(PREMADE_MODELS premade_model, const float& scale)
	: num_vertices(0), num_triangles(0)
{
	switch (premade_model)
	{
		case PREMADE_MODELS::UNIT_CUBE_REPEAT_TEXTURE:
		{
			num_vertices = 24;
			num_triangles = 12;
			
			vertex_data.reserve(num_vertices);
			triangles.reserve(num_triangles);
			
			// Setting up faces
			// Forward
			int vind = 0;
			vertex_data.emplace_back( 0.5f, 0.5f, 0.5f,   1.0f, 0.0f,  0,  0.0f, 0.0f, 1.0f);
			vertex_data.emplace_back(-0.5f, 0.5f, 0.5f,   0.0f, 0.0f,  0,  0.0f, 0.0f, 1.0f);
			vertex_data.emplace_back(-0.5f, -0.5f, 0.5f,  0.0f, 1.0f,  0,  0.0f, 0.0f, 1.0f);
			vertex_data.emplace_back( 0.5f, -0.5f, 0.5f,  1.0f, 1.0f,  0,  0.0f, 0.0f, 1.0f);
			triangles.emplace_back(vind, vind + 1, vind + 2);
			triangles.emplace_back(vind, vind + 2, vind + 3);
			
			// Back
			vind += 4;
			vertex_data.emplace_back( 0.5f, 0.5f, -0.5f,   0.0f, 0.0f,  0,  0.0f, 0.0f, -1.0f);
			vertex_data.emplace_back(-0.5f, 0.5f, -0.5f,   1.0f, 0.0f,  0,  0.0f, 0.0f, -1.0f);
			vertex_data.emplace_back(-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0,  0.0f, 0.0f, -1.0f);
			vertex_data.emplace_back( 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0,  0.0f, 0.0f, -1.0f);
			triangles.emplace_back(vind + 2, vind + 1, vind);
			triangles.emplace_back(vind + 3, vind + 2, vind);
			
			// Left
			vind += 4;
			vertex_data.emplace_back(0.5f, 0.5f,  -0.5f,  1.0f, 0.0f,  0,  1.0f, 0.0f, 0.0f);
			vertex_data.emplace_back(0.5f, 0.5f,  0.5f,   0.0f, 0.0f,  0,  1.0f, 0.0f, 0.0f);
			vertex_data.emplace_back(0.5f, -0.5f, 0.5f,   0.0f, 1.0f,  0,  1.0f, 0.0f, 0.0f);
			vertex_data.emplace_back(0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0,  1.0f, 0.0f, 0.0f);
			triangles.emplace_back(vind, vind + 1, vind + 2);
			triangles.emplace_back(vind, vind + 2, vind + 3);
			
			// Right
			vind += 4;
			vertex_data.emplace_back(-0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  0,  -1.0f, 0.0f, 0.0f);
			vertex_data.emplace_back(-0.5f, 0.5f,  -0.5f, 0.0f, 0.0f,  0,  -1.0f, 0.0f, 0.0f);
			vertex_data.emplace_back(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0,  -1.0f, 0.0f, 0.0f);
			vertex_data.emplace_back(-0.5f, -0.5f, 0.5f,  1.0f, 1.0f,  0,  -1.0f, 0.0f, 0.0f);
			triangles.emplace_back(vind, vind + 1, vind + 2);
			triangles.emplace_back(vind, vind + 2, vind + 3);
			
			// Top
			vind += 4;
			vertex_data.emplace_back( 0.5f, 0.5f, -0.5f, 1.0f, 0.0f,  0,  0.0f, 1.0f, 0.0f);
			vertex_data.emplace_back(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f,  0,  0.0f, 1.0f, 0.0f);
			vertex_data.emplace_back(-0.5f, 0.5f, 0.5f,  0.0f, 1.0f,  0,  0.0f, 1.0f, 0.0f);
			vertex_data.emplace_back( 0.5f, 0.5f, 0.5f,  1.0f, 1.0f,  0,  0.0f, 1.0f, 0.0f);
			triangles.emplace_back(vind, vind + 1, vind + 2);
			triangles.emplace_back(vind, vind + 2, vind + 3);
			
			// Bottom
			vind += 4;
			vertex_data.emplace_back( 0.5f, -0.5f, 0.5f,   1.0f, 0.0f,  0,  0.0f, -1.0f, 0.0f);
			vertex_data.emplace_back(-0.5f, -0.5f, 0.5f,   0.0f, 0.0f,  0,  0.0f, -1.0f, 0.0f);
			vertex_data.emplace_back(-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0,  0.0f, -1.0f, 0.0f);
			vertex_data.emplace_back( 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0,  0.0f, -1.0f, 0.0f);
			triangles.emplace_back(vind, vind + 1, vind + 2);
			triangles.emplace_back(vind, vind + 2, vind + 3);
			
			break;
		}
		default:
			assert(0);
	}

	if (scale != 1.0f)
		for (auto& v : vertex_data)
			v.pos *= scale;

	populate_all_arrays_from_vertex_data();
}
Model::Model(const std::string& file_name, const float& xz_size, const float& max_height, const float& u, const float& v)
{
	// Vertically flips all loaded textures
	stbi_set_flip_vertically_on_load(true);

	uint32_t width, height, components;
	uint16_t* hgt_map = stbi_load_16(("heightmaps/" + file_name).c_str(), (int*)&width, (int*)&height, (int*)&components, 0);
	assert(hgt_map && "Error loading heightmap file!");
	assert(width == height && width > 1);

	num_vertices = width * height;
	num_triangles = (width - 1) * (height - 1) * 2;

	const float y_coord_ratio = max_height * (1.0f / UINT_LEAST16_MAX);
	const float xz_step = xz_size / (width - 1);
	const float u_step = u / (width - 1);
	const float v_step = v / (height - 1);
	const float pos_x_orig = xz_size * -0.5f;
	const float pos_z_orig = xz_size * -0.5f;
	const float uv_u_orig = -u * 0.5f;
	const float uv_v_orig = -v * 0.5f;

	// Calculate the vertices and UVs
	vertex_data.reserve(num_vertices);
	for (uint32_t row = 0; row < height; row++)
		for (uint32_t col = 0; col < width; col++)
			vertex_data.emplace_back(pos_x_orig + (col * xz_step), hgt_map[col + row * width] * y_coord_ratio, pos_z_orig + (row * xz_step),
									 uv_u_orig + (col * u_step), uv_v_orig + (row * v_step));
	
	// Calculate the normals for the inner vertices
	for (uint32_t row = 1; row < height - 1; row++)
		for (uint32_t col = 1; col < width - 1; col++) 
		{
			const glm::vec3 temp_normals[]{
				glm::normalize(glm::cross(vertex_data[(col + 1) + (row - 1) * width].pos - vertex_data[col + row * width].pos, vertex_data[col + (row - 1) * width].pos - vertex_data[col + row * width].pos)),
				glm::normalize(glm::cross(vertex_data[(col + 1) + row * width].pos - vertex_data[col + row * width].pos, vertex_data[(col + 1) + (row - 1) * width].pos - vertex_data[col + row * width].pos)),
				glm::normalize(glm::cross(vertex_data[col + (row + 1) * width].pos - vertex_data[col + row * width].pos, vertex_data[(col + 1) + row * width].pos - vertex_data[col + row * width].pos)),
				glm::normalize(glm::cross(vertex_data[(col - 1) + (row + 1) * width].pos - vertex_data[col + row * width].pos, vertex_data[col + (row + 1) * width].pos - vertex_data[col + row * width].pos)),
				glm::normalize(glm::cross(vertex_data[(col - 1) + row * width].pos - vertex_data[col + row * width].pos, vertex_data[(col - 1) + (row + 1) * width].pos - vertex_data[col + row * width].pos)),
				glm::normalize(glm::cross(vertex_data[col + (row - 1) * width].pos - vertex_data[col + row * width].pos, vertex_data[(col - 1) + row * width].pos - vertex_data[col + row * width].pos))
			};

			glm::vec3 normals_accu(0.0f);
			for (const auto& x : temp_normals)
				normals_accu += x;

			vertex_data[col + row * width].normal = glm::normalize(normals_accu);
		}

	// Create the triangle indices
	triangles.reserve(num_triangles);
	for (uint32_t row = 1; row < height; row++)
		for (uint32_t col = 0; col < width - 1; col++)
		{
			triangles.emplace_back(col + row * width, (col + 1) + (row - 1) * width, col + (row - 1) * width);
			triangles.emplace_back(col + row * width, (col + 1) + row * width, (col + 1) + (row - 1) * width);
		}
	
	// Calculating the tangents and bitangents for the vertices
	calculate_tangents();

	// Populates the rest of the data members of the Model class from the vertex data
	populate_all_arrays_from_vertex_data();

	// Frees up the memory used by stb
	stbi_image_free(hgt_map);
}
Model::Model(const float& xz_size, const float& u, const float& v)
	: num_vertices(4), num_triangles(2)
{
	const auto xz_size_half = xz_size * 0.5f;
	const auto u_half = u * 0.5f;
	const auto v_half = v * 0.5f;
	vertex_data.emplace_back(-xz_size_half, 0.0f, -xz_size_half, -u_half, -v_half, 0,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f);
	vertex_data.emplace_back(-xz_size_half, 0.0f,  xz_size_half, -u_half,  v_half, 0,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f);
	vertex_data.emplace_back( xz_size_half, 0.0f,  xz_size_half,  u_half,  v_half, 0,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f);
	vertex_data.emplace_back( xz_size_half, 0.0f, -xz_size_half,  u_half, -v_half, 0,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f);
	triangles.emplace_back(0, 1, 2);
	triangles.emplace_back(0, 2, 3);

	for (const auto& vertex : vertex_data)
	{
		vertices.emplace_back(vertex.pos);
		uvs.emplace_back(vertex.uv);
		normals.emplace_back(vertex.normal);
		tangents.emplace_back(vertex.tangent);
		bitangents.emplace_back(vertex.bitangent);
		joint_ids.emplace_back(vertex.joint_ids);
		joint_weights.emplace_back(vertex.joint_weights);
	}
	load_GPU_data(*this);
}

Model::Model(Model&& o) noexcept
	: vao(o.vao), vbo(o.vbo), ebo(o.ebo), num_vertices(o.num_vertices), num_triangles(o.num_triangles), vertex_data(std::move(o.vertex_data)),
	vertices(std::move(o.vertices)), uvs(std::move(o.uvs)), 
	normals(std::move(o.normals)), tangents(std::move(o.tangents)), bitangents(std::move(o.bitangents)), 
	joint_ids(std::move(o.joint_ids)), joint_weights(std::move(o.joint_weights)), triangles(std::move(o.triangles))
{
	o.vao = 0;
	o.vbo = 0;
	o.ebo = 0;
}
Model& Model::operator=(Model&& o)
{
	vao = o.vao;
	vbo = o.vbo;
	ebo = o.ebo;
	num_vertices = o.num_vertices;
	num_triangles = o.num_triangles;
	vertex_data = std::move(o.vertex_data);
	vertices = std::move(o.vertices);
	uvs = std::move(o.uvs);
	normals = std::move(o.normals);
	tangents = std::move(o.tangents);
	bitangents = std::move(o.bitangents);
	joint_ids = std::move(o.joint_ids);
	joint_weights = std::move(o.joint_weights);
	triangles = std::move(o.triangles);

	o.vao = 0;
	o.vbo = 0;
	o.ebo = 0;
	return *this;
}
Model::~Model()
{
	// By default OpenGL silently ignores buffer 0, but this destructor is called in a separate thread so I can't call gl functions at all
	if (vao != 0) 
	{
		glDeleteBuffers(2, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
}

void Model::Bind() const
{
	glBindVertexArray(vao);
}

const GLuint& Model::GetVAO() const
{
	return vao;
}
const GLuint& Model::GetVBO() const
{
	return vbo;
}
const GLuint& Model::GetEBO() const
{
	return ebo;
}
const uint32_t& Model::GetNumVertices() const
{
	return num_vertices;
}
const uint32_t& Model::GetNumTriangles() const
{
	return num_triangles;
}
const std::vector<VertexTypes::Vertex>& Model::GetVertexData() const
{
	return vertex_data;
}
const std::vector<glm::vec3>& Model::GetVertices() const
{
	return vertices;
}
const std::vector<glm::vec2>& Model::GetUVs() const
{
	return uvs;
}
const std::vector<glm::vec3>& Model::GetNormals() const
{
	return normals;
}
const std::vector<glm::vec3>& Model::GetTangents() const
{
	return tangents;
}
const std::vector<glm::vec3>& Model::GetBitangents() const
{
	return bitangents;
}
const std::vector<glm::uvec4>& Model::GetJointIDs() const
{
	return joint_ids;
}
const std::vector<glm::vec4>& Model::GetJointWeights() const
{
	return joint_weights;
}
const std::vector<VertexTypes::VertexTriangle>& Model::GetTriangles() const
{
	return triangles;
}

inline void Model::calculate_tangents()
{
	for (uint32_t i = 0; i < num_triangles; i++)
	{
		auto& v0 = vertex_data[triangles[i].i0];
		auto& v1 = vertex_data[triangles[i].i1];
		auto& v2 = vertex_data[triangles[i].i2];

		const auto deltaPos1 = v1.pos - v0.pos;
		const auto deltaPos2 = v2.pos - v0.pos;

		const auto deltaUV1 = v1.uv - v0.uv;
		const auto deltaUV2 = v2.uv - v0.uv;

		const float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		if (!std::isnan(r))
		{
			const glm::vec3 tangent = glm::vec3(deltaPos1.x * deltaUV2.y - deltaPos2.x * deltaUV1.y, deltaPos1.y * deltaUV2.y - deltaPos2.y * deltaUV1.y, deltaPos1.z * deltaUV2.y - deltaPos2.z * deltaUV1.y) 
									* r;
			const glm::vec3 bitangent = glm::vec3(deltaPos2.x * deltaUV1.x - deltaPos1.x * deltaUV2.x, deltaPos2.y * deltaUV1.x - deltaPos1.y * deltaUV2.x, deltaPos2.z * deltaUV1.x - deltaPos1.z * deltaUV2.x)
									* r;
			v0.tangent += tangent;
			v0.bitangent += bitangent;
			v1.tangent += tangent;
			v1.bitangent += bitangent;
			v2.tangent += tangent;
			v2.bitangent += bitangent;
		}
	}

	for (auto& v : vertex_data)
	{
		v.tangent = glm::normalize(v.tangent);
		v.bitangent = glm::normalize(v.bitangent);
	}
		
}

void Model::populate_all_arrays_from_vertex_data()
{
	vertices.reserve(num_vertices);
	normals.reserve(num_vertices);
	tangents.reserve(num_vertices);
	bitangents.reserve(num_vertices);
	joint_ids.reserve(num_vertices);
	joint_weights.reserve(num_vertices);
	for (const auto& v : vertex_data)
	{
		vertices.emplace_back(v.pos);
		uvs.emplace_back(v.uv);
		normals.emplace_back(v.normal);
		tangents.emplace_back(v.tangent);
		bitangents.emplace_back(v.bitangent);
		joint_ids.emplace_back(v.joint_ids);
		joint_weights.emplace_back(v.joint_weights);
	}
}

void Model::load_GPU_data(Model& mod)
{
	glGenVertexArrays(1, &mod.vao);
	glBindVertexArray(mod.vao);

	glGenBuffers(2, &mod.vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, mod.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexTypes::Vertex) * mod.num_vertices, mod.vertex_data.data(), GL_STATIC_DRAW);

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
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTypes::Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(uint32_t) + 4 + sizeof(glm::vec3)));

	// Bitangent
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTypes::Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(uint32_t) + 4 + sizeof(glm::vec3) * 2));

	// Joint IDs
	glEnableVertexAttribArray(6);
	glVertexAttribIPointer(6, 4, GL_UNSIGNED_INT, sizeof(VertexTypes::Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(uint32_t) + 4 + sizeof(glm::vec3) * 3));

	// Joint Weights
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(VertexTypes::Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(uint32_t) + 4 + sizeof(glm::vec3) * 3 + sizeof(glm::uvec4)));

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(VertexTypes::VertexTriangle) * mod.GetNumTriangles(), mod.triangles.data(), GL_STATIC_DRAW);
}