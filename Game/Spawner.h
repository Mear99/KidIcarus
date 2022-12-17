#pragma once
#include "GameObject.h"
#include "EnemyManager.h"
#include "Level.h"
class Enemy;

enum class SpawnerState{waiting, spawning, cooldown, disabled};

class Spawner final : public GameObject
{
	public:
		explicit Spawner(const Point2f& screenOffset, int amount, EnemyType type, const Rectf& spawnRegion = Rectf{ 0,0,0,0 });

		Spawner(const Spawner& other) = delete;
		Spawner(Spawner&& other) = delete;
		Spawner& operator=(const Spawner& other) = delete;
		Spawner& operator=(Spawner&& other) = delete;

		~Spawner();

		void Update(float elapsedSec, EnemyManager& manager, const Rectf& cameraY, const Level& level);
		void EnemyDespawned();
		void DeactivateSpawner();
		void ActivateSpawner();

	private:
		Rectf m_SpawnRegion;
		Point2f m_ScreenOffset;
		int m_Amount;
		int m_EnemiesAlive;
		Enemy** m_pEnemies;
		EnemyType m_Type;
		const float m_SpawnDelay;
		const float m_CoolDownTime;
		float m_AccuTime;
		SpawnerState m_State;
		int m_SuccessfullSpawns;

		void SpawnEnemy(EnemyManager& manager, const Level& level);
		void CheckSpawnArea(const Rectf& camera);
};

