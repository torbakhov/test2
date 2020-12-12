#pragma once

#include "common.h"

class Camera {
public:
	glm::vec3 pos, dir, right, up;
	float moveSpeed;

	glm::mat4 viewMatrix() const
	{
		glm::mat4 view = glm::lookAt(pos, pos + dir, glm::vec3(0, 1, 0));
		return view;
	}
	glm::mat4 projectionMatrix() const
	{
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 300.0f);
		return projection;
	}

	void moveCam(GLFWwindow* window, float delta);
};