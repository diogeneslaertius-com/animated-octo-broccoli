#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 position);

	glm::mat4 ProcessInput(GLFWwindow* window, float deltaTime);
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(GLFWwindow* window, float fov, float nearPlane, float farPlane) const;
	glm::vec3 GetPosition() const { return m_Position; }
	glm::vec3 GetFront() const { return m_Front; }

	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

	float m_LastX = 400.0f;
	float m_LastY = 300.0f; // Начальная позиция Y (центр экрана)
	bool m_FirstMouse = true; //

private:
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	float m_Yaw;
	float m_Pitch;
	float m_MouseSensitivity = 0.1f; // Чувствительность

	void updateCameraVectors();
};