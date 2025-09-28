#pragma once
#include "Enemy.h"
#include <vector>

class EnemyPool {
public:
	EnemyPool(size_t size) { enemies.resize(size); }

	Enemy* Spawn(const glm::vec3& pos);

	void UpdateAll(float deltaTime, const glm::vec3& playerPos);

	void SpawnWaveCircle(int count, const glm::vec3& center, float radius, float jitter = 0.5f);

	std::vector<Enemy>& GetEnemies() { return enemies; }
	const std::vector<Enemy>& GetEnemies() const { return enemies; }

private:
	std::vector<Enemy> enemies;
};
