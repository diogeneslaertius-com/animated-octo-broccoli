#include "Camera.h"

Camera::Camera(glm::vec3 position)
	: m_Position(position), m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_Up(
		glm::vec3(0.0f, 1.0f, 0.0f)),
	m_Yaw(-90.0f), m_Pitch(0.0f)
{
	updateCameraVectors(); // Вызов, чтобы m_Front был корректен
}

// ... Ваш ProcessInput и GetViewMatrix остаются прежними ...

// Логика обновления векторов
void Camera::updateCameraVectors()
{
	glm::vec3 front;
	// Вычисление компонент X и Z (основано на Yaw)
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	// Вычисление компонента Y (основано на Pitch)
	front.y = sin(glm::radians(m_Pitch));
	// Вычисление компонента Z (основано на Yaw)
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	m_Front = glm::normalize(front);
	// (Обычно здесь также пересчитывают m_Right и m_Up, но для m_Up
	// достаточно фиксированного вектора (0, 1, 0) для простой FPS-камеры).
}

// Логика обработки смещения мыши
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_Yaw += xoffset;
	m_Pitch += yoffset;

	// Ограничение тангажа (Pitch) для предотвращения "переворота" камеры
	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	// Пересчитываем m_Front с учетом новых углов
	updateCameraVectors();
}

glm::mat4 Camera::ProcessInput(GLFWwindow* window, float deltaTime)
{
	float camSpeed = 2.5f * deltaTime;

	// Передвижение
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_Position += camSpeed * m_Front;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_Position -= camSpeed * m_Front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * camSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * camSpeed;

	// Возвращаем View Matrix
	return GetViewMatrix();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::GetProjectionMatrix(GLFWwindow * window, float fov, float nearPlane, float farPlane) const
{
	int w = 0, h = 0;
	glfwGetWindowSize(window, &w, &h);

	// Проверка на деление на ноль
	if (h == 0) h = 1;

	float aspectRatio = static_cast<float>(w) / static_cast<float>(h);

	return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}