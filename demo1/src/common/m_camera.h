#ifndef  __M_CAMERA_H__
#define  __M_CAMERA_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../_morisa.h"
#include "../utils/m_math.h"

MORISA_NAMESPACE_BEGIN

class m_camera
{
public:
	typedef glm::vec3 vec;
	m_camera() {}

	m_camera(vec position,vec worldUp, float pitch, float yaw):
		position(position), worldUp(worldUp), pitch(pitch), yaw(yaw)
	{ updateCameraVectors(); }

	inline const glm::mat4 getViewMat() const noexcept
	{
		return glm::lookAt(position, position + front, up);
	}

	void keyEvent(GLFWwindow* window, float deltaTime) 
	{
		float _speed_ = speed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
			return;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			position += front * _speed_;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			position -= front * _speed_;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			position += right * _speed_;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			position -= right * _speed_;
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			position -= worldUp * _speed_;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			position += worldUp * _speed_;

		updateCameraVectors();
	}

	void mouseCallBack(GLFWwindow* window, float posX, float posY, bool usePitchLock = true) 
	{
		if (firstMouse) 
		{
			lastX = posX;
			lastY = posY;
			firstMouse = false;
		}
		float offsetX = posX - lastX;
		float offsetY = lastY - posY;
		yaw += offsetX * mouseSensitivity;
		pitch += offsetY * mouseSensitivity;
		if (usePitchLock) pitch = clamp(pitch, 89.0f, -89.0f);
		lastX = posX;
		lastY = posY;
		updateCameraVectors();
	}

	void scrollCallBack(GLFWwindow* window, float Xoffset, float Yoffset)
	{
		fov = clamp(fov - Yoffset, 45.0f, 1.0f);
		updateCameraVectors();
	}

	inline const float getFov()const noexcept { return fov; }

private:
	vec position{ vec(0.0f, 0.0f, 10.0f) };
	vec worldUp{ vec(0.0f, 1.0f, 0.0f) };

	vec front{ vec(0.0f, 0.0f, -1.0f) };
	vec up{ vec(0.0f, 0.0f, 0.0f) };
	vec right{ vec(0.0f, 0.0f, 0.0f) };

	float yaw{-90.0f};
	float pitch{0.0f};

	float speed{25.0f};
	float mouseSensitivity{0.01f};
	float fov{45.0f};

	float deltaTime{ 0.0f };
	float lastTime{ 0.0f };

	float lastX{ 0.0f };
	float lastY{ 0.0f };
	bool firstMouse = true;

	inline void updateCameraVectors() 
	{
		front = glm::normalize(glm::vec3(
			cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
			sin(glm::radians(pitch)),
			sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
};


MORISA_NAMESPACE_END

#endif 

