#include "gpch.h"
#include "Model.h"
#include "GlacierFileHdr.h"
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

	std::vector<glm::vec3> vertices(pos, pos + num_vertices);
	std::vector<glm::vec2> uvs(uv, uv + num_vertices);
	std::vector<glm::vec3> normals(norm, norm + num_vertices);

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

	const auto& joint_ids_data = data.find("JOINTS_0");
	if (joint_ids_data != data.end())
	{
		assert(data.find("WEIGHTS_0") != data.end());
		const uint8_t* const joint = (const uint8_t*)joint_ids_data->second.data;
		std::vector<glm::uvec4> joint_ids;
		joint_ids.reserve(num_vertices);
		for (uint32_t i = 0; i < num_vertices; i++)
			joint_ids.emplace_back(joint[i * 4], joint[i * 4 + 1], joint[i * 4 + 2], joint[i * 4 + 3]);
		
		const glm::vec4* const weights = (const glm::vec4*)data.find("WEIGHTS_0")->second.data;
		std::vector<glm::vec4> joint_weights(weights, weights + num_vertices);

		for (uint32_t i = 0; i < num_vertices; i++)
			vertex_data.emplace_back(vertices[i], uvs[i], 0, normals[i], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), joint_ids[i], joint_weights[i]);
	
		assert(j.find("skins") != j.end());
		const uint32_t inv_bind_matrix_accessor_location = j["skins"][0]["inverseBindMatrices"];
		assert(j.find("accessors") != j.end());
		auto& inverse_bind_matrix_accessor = j["accessors"][inv_bind_matrix_accessor_location];
		const uint32_t& inv_bind_matrix_bufferView = inverse_bind_matrix_accessor["bufferView"];
		const uint32_t& inv_bind_matrix_count = inverse_bind_matrix_accessor["count"];
		assert(j.find("bufferViews") != j.end());
		auto& inverse_bind_matrix_bufferView = j["bufferViews"][inv_bind_matrix_bufferView];
		uint32_t byteOffset = inverse_bind_matrix_bufferView["byteOffset"];
		inverse_bind_matrices = std::vector<glm::mat4>((const glm::mat4*)&buffer_data[byteOffset], (const glm::mat4*)&buffer_data[byteOffset] + inv_bind_matrix_count);
	
		num_joints = (uint32_t)j["skins"][0]["joints"].size();
	}
	else
	{
		for (uint32_t i = 0; i < num_vertices; i++)
			vertex_data.emplace_back(vertices[i], uvs[i], 0, normals[i], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::uvec4(0,0,0,0), glm::vec4(1.0f,1.0f,1.0f,1.0f));
	}

	calculate_tangents();
	calculate_bsphere();
}
Model::Model(const std::string& file_name, bool)
{
	std::ifstream file(file_name, std::ifstream::binary);
	if (!file.is_open())
		throw std::exception(("File " + file_name + " not found!").c_str());

	file.seekg(0, std::ifstream::end);
	const size_t file_size = file.tellg();
	file.seekg(0, std::ifstream::beg);
	
	uint8_t* buf = new uint8_t[file_size];
	file.read((char*)buf, file_size);
	file.close();

	/*
	* Parsing out the data
	*/
	int curr_index;
	GlacierFileHdr hdr;

	// Read the asset_name
	for (curr_index = 0; buf[curr_index] != '\0'; curr_index++)
	{
		hdr.asset_name += buf[curr_index];
	}
	curr_index++;

	// Read some data about the model
	memcpy_s(&hdr.type, sizeof(uint32_t) * 5, &buf[curr_index], sizeof(uint32_t) * 5);
	num_vertices = hdr.num_vertices;
	num_triangles = hdr.num_indices / 3;
	num_joints = hdr.num_joints;
	curr_index += sizeof(uint32_t) * 5;

	// Read in the actual model data
	vertex_data = std::vector<VertexTypes::Vertex>((VertexTypes::Vertex*)&buf[curr_index], (VertexTypes::Vertex*)&buf[curr_index] + num_vertices);
	curr_index += sizeof(VertexTypes::Vertex) * num_vertices;

	// Read in the index data
	triangles = std::vector<VertexTypes::VertexTriangle>((VertexTypes::VertexTriangle*)&buf[curr_index], (VertexTypes::VertexTriangle*)&buf[curr_index] + num_triangles);
	curr_index += sizeof(VertexTypes::VertexTriangle) * num_triangles;

	// Read in the inverse bind matrices
	if (num_joints > 0)
	{
		inverse_bind_matrices = std::vector<glm::mat4>((glm::mat4*)&buf[curr_index], (glm::mat4*)&buf[curr_index] + num_joints);
		curr_index += sizeof(glm::mat4) * num_joints;
	}
	assert(file_size == curr_index);

	delete[] buf;

	calculate_tangents();
	calculate_bsphere();
}
Model::Model(const std::vector<VertexTypes::Vertex>& verts, const std::vector<VertexTypes::VertexTriangle>& triangles)
	: vertex_data(verts), triangles(triangles), num_vertices((uint32_t)verts.size()), num_triangles((uint32_t)triangles.size())
{
	assert(triangles.size() > 0 && verts.size() > 0);
	load_gpu_data();
	calculate_bsphere();
}
Model::Model(PREMADE_MODELS premade_model, const float& scale)
	: num_vertices(0), num_triangles(0), num_joints(0)
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
	{
		for (auto& v : vertex_data)
			v.pos *= scale;
	}

	calculate_bsphere();
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

	// Frees up the memory used by stb
	stbi_image_free(hgt_map);
}
Model::Model(const uint32_t& v_slices, const uint32_t& h_slices)
	: num_vertices((v_slices + 1) * h_slices + (2 * v_slices)), num_triangles((v_slices * 2) + ((h_slices - 1) * v_slices * 2)),
	bsphere_center(0.0f), bsphere_radius(1.0f)
{
	uint32_t i, j;

	vertex_data.reserve(num_vertices);
	for (i = 0; i < num_vertices; i++)
		vertex_data.emplace_back();

	triangles.reserve(num_triangles);
	for (i = 0; i < num_triangles; i++)
		triangles.emplace_back();

	// Vertices
	const glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::pi<float>() / (float)(h_slices + 1), glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::two_pi<float>() / (float)v_slices, glm::vec3(0.0f, 1.0f,0.0f));
	const glm::vec2 deltaUV{ 1.0f / h_slices, 1.0f / (v_slices + 1) };

	glm::vec4 currentVect(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec2 currentUV(0.0f, 0.0f);

	for (i = 0; i < v_slices; i++)
		vertex_data[i].set(glm::vec3(0.0f, 1.0f, 0.0f), currentUV + glm::vec2(0.5f / v_slices + i / (float)v_slices, 0.0f), currentVect);

	currentVect = rotationX * currentVect;
	currentUV = { 0.0f, deltaUV.y };
	for (i = 1; i < num_vertices - v_slices * 2 + 1; i++)
	{
		vertex_data[i + v_slices - 1].set(currentVect, currentUV, currentVect);
		currentVect = rotationY * currentVect;
		currentUV.x += deltaUV.x;
		if (i % (v_slices + 1) == 0)
		{
			vertex_data[i + v_slices - 1].set(vertex_data[(i + v_slices - 1) - v_slices].pos, vertex_data[i + v_slices - 1].uv, vertex_data[(i + v_slices - 1) - v_slices].normal);
			currentVect = rotationX * glm::vec4(vertex_data[(i + v_slices - 1) - v_slices].pos, 1.0f);
			currentUV.y += deltaUV.y;
			currentUV.x = 0.0f;
		}
	}
	currentVect = glm::vec4{ 0.0f, -1.0f, 0.0f, 1.0f };
	for (i = num_vertices - v_slices, j = 0; i < num_vertices; i++, j++)
		vertex_data[i].set(currentVect, currentUV + glm::vec2(0.5f / v_slices + j / (float)v_slices, 0.0f), currentVect);

	// Triangles
	for (i = 0, j = v_slices; i < v_slices; i++, j++)
	{
		triangles[i] = { j, j + 1, i };
	}

	j -= v_slices;
	for (++j; i < num_triangles - v_slices; i += 2, j++)
	{
		triangles[i] = { j + v_slices, j - (v_slices + 1) + 1 + v_slices, j - (v_slices + 1) + v_slices };
		triangles[i + 1] = { j + 1 + v_slices, j - (v_slices + 1) + 1 + v_slices, j + v_slices };
		if ((j + 2) % (v_slices + 1) == 0)
			j++;
	}
	j += v_slices;
	
	j -= (v_slices + 1);
	for (uint32_t z = 0; i < num_triangles; i++, j++, z++)
	{
		triangles[i] = { j + 1, j, num_vertices - v_slices + z };
	}

	calculate_tangents();
}
Model::Model(const float& xz_size, const float& u, const float& v)
	: num_vertices(4), num_triangles(2), num_joints(0), bsphere_center(0.0f), bsphere_radius(0.0f)
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
}

Model::Model(Model&& o) noexcept
	: vao(o.vao), vbo(o.vbo), ebo(o.ebo),
	num_vertices(o.num_vertices), num_triangles(o.num_triangles), num_joints(o.num_joints),
	bsphere_center(o.bsphere_center), bsphere_radius(o.bsphere_radius),
	vertex_data(std::move(o.vertex_data)), inverse_bind_matrices(std::move(o.inverse_bind_matrices)),
	triangles(std::move(o.triangles))
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
	num_joints = o.num_joints;
	bsphere_center = o.bsphere_center;
	bsphere_radius = o.bsphere_radius;
	vertex_data = std::move(o.vertex_data);
	inverse_bind_matrices = std::move(o.inverse_bind_matrices);
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
const uint32_t& Model::GetNumBones() const
{
	return num_joints;
}
const glm::vec3& Model::GetBSphereCenter() const
{
	return bsphere_center;
}
const float& Model::GetBSphereRadius() const
{
	return bsphere_radius;
}
const std::vector<VertexTypes::Vertex>& Model::GetVertexData() const
{
	return vertex_data;
}
const std::vector<glm::mat4>& Model::GetInverseBindMatrices() const
{
	return inverse_bind_matrices;
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
inline void Model::calculate_bsphere()
{
	// Ritter's Algorithm
	const glm::vec3& X = vertex_data[0].pos;
	glm::vec3 Y;
	float largest_distance_sqr = 0.0f;
	for (auto it = vertex_data.cbegin() + 1; it != vertex_data.cend(); ++it)
	{
		const glm::vec3& diff = it->pos - X;
		const float& d = glm::dot(diff, diff);
		if (d > largest_distance_sqr)
		{
			largest_distance_sqr = d;
			Y = it->pos;
		}
	}

	glm::vec3 Z;
	largest_distance_sqr = 0.0f;
	for (auto it = vertex_data.cbegin(); it != vertex_data.cend(); ++it)
	{
		const glm::vec3& diff = it->pos - Y;
		const float& d = glm::dot(diff, diff);
		if (d > largest_distance_sqr)
		{
			largest_distance_sqr = d;
			Z = it->pos;
		}
	}

	bsphere_center = 0.5f * (Z + Y);
	bsphere_radius = 0.5f * sqrtf(largest_distance_sqr);
	
	float bsphere_radius_sqr = bsphere_radius * bsphere_radius;
	for (const auto& vert : vertex_data)
	{
		const glm::vec3& diff = vert.pos - bsphere_center;
		const float& dist = glm::dot(diff, diff);
		if (dist > bsphere_radius_sqr)
		{
			const float d_half = (sqrtf(dist) - bsphere_radius) * 0.5f;
			bsphere_radius += d_half;
			bsphere_radius_sqr = bsphere_radius * bsphere_radius;
			bsphere_center += glm::normalize(diff) * d_half;
		}
	}
}

void Model::load_gpu_data()
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