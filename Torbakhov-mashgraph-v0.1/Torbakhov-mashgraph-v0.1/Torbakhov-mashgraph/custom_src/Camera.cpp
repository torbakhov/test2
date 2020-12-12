#include "../custom_include/Camera.h"

void Camera::moveCam(GLFWwindow* window, float delta) {
	right = glm::normalize(cross(dir, glm::vec3(0, 1, 0)));
	up = glm::normalize(cross(right, dir));
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		pos += moveSpeed * delta * dir;
	}
	//back
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		pos -= moveSpeed * delta * dir;
	}
	//right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		pos += moveSpeed * delta * right;
	}
	//left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		pos -= moveSpeed * delta * right;
	}
	//turn right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		dir = glm::normalize(glm::rotate(dir, -delta, up));
	}
	//turn left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		dir = glm::normalize(glm::rotate(dir, delta, up));
	}
	//turn up
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		dir = glm::normalize(glm::rotate(dir, delta, right));
	}
	//turn down
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		dir = glm::normalize(glm::rotate(dir, -delta, right));
	}
}