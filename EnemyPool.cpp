#include "EnemyPool.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

Enemy* EnemyPool::Spawn(const glm::vec3& pos) {
	for (auto& e : enemies) {
		if (!e.IsActive()) {
			e.Reset(pos);
			return &e;
		}
	}
	return nullptr;
}

void EnemyPool::UpdateAll(float deltaTime, const glm::vec3& playerPos) {
	for (auto& e : enemies) {
		if (!e.IsActive()) continue;
		e.Update(deltaTime, playerPos);
	}
}

