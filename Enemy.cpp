#include "Enemy.h"
#include <glm/gtc/matrix_transform.hpp>

Enemy::Enemy()
	: active(false), hp(100.0f), maxHp(100.0f),
	position(0.0f), speed(2.0f), shootTimer(0.0f) {}

void Enemy::Reset(const glm::vec3& spawnPos) {
	active = true;
	hp = maxHp;
	position = spawnPos;
	shootTimer = 0.0f;
}

void Enemy::TakeDamage(float dmg) {
	hp -= dmg;
	if (hp <= 0.0f) {
		active = false;
	}
}

void Enemy::Update(float deltaTime, const glm::vec3& playerPos) {
	if (!active) return;

	glm::vec3 dir = glm::normalize(playerPos - position);
	position += dir * speed * deltaTime;

	shootTimer -= deltaTime;
	if (shootTimer <= 0.0f) {
		// TODO: ShootAt(position, playerPos);
		shootTimer = 2.0f;
	}
}

glm::mat4 Enemy::GetModelMatrix() const {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(1.0f)); // размер кубика
	return model;
}

glm::vec4 Enemy::GetColor() const {
	// Цвет зависит от HP
	float hpRatio = hp / maxHp;
	return glm::vec4(1.0f - hpRatio, hpRatio, 0.0f, 1.0f); // от красного к зелёному
}
