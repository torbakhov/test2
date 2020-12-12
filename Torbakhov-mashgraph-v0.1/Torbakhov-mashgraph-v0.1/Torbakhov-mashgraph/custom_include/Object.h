#pragma once

#include "common.h"
#include "Mesh.h"

#include "ShaderProgram.h"
#include "Camera.h"
#include "Texture.h"

class Object {
public:
	Mesh* mesh;
	Texture* diffuseMap, * normalMap, * heightMap, * shadowMap;
	ShaderProgram* shader;
	glm::vec3 position;
	glm::quat rotation;
	float scale;

	Object() : mesh(NULL), shader(NULL), diffuseMap(NULL), normalMap(NULL), heightMap(NULL), scale(1.0f) {}

	void drawObject(Camera* camera, ShaderProgram* overrideShader = NULL, Camera* shadowCamera = NULL) const;
};
