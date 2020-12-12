#pragma once

#include "common.h"

struct MeshVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 texCoords;
};

class Mesh {
	unsigned vbo, ebo, vao;

public:
	std::vector<MeshVertex> vertices;
	std::vector<unsigned> indices;

	Mesh() : vbo(0), ebo(0), vao(0)
	{
	}
	~Mesh();

	void render() const;

	void load(const char* path);

	void setup();

	void updatePositions();
};