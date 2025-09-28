#pragma once
#include <glm/glm.hpp>

class Beam {
public:
	Beam(const glm::vec3& start, const glm::vec3& dir, float length, float lifeTime, const glm::vec3& color);

	void Update(float deltaTime);
	void Draw();

	bool IsAlive() const { return m_LifeTime > 0.0f; }

	glm::mat4 GetModelMatrix() const;
	glm::vec3 GetColor() const { return m_Color; }
private:
	glm::vec3 m_Start;
	glm::vec3 m_Dir;
	glm::vec3 m_Color;

	float m_Length;
	float m_LifeTime; // сколько ещё жить трассеру
};
