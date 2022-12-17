#pragma once
#include <vector>
#include "Spawner.h"
#include "Level.h"

class SpawnerManager final
{
	public:
		static SpawnerManager* GetInstance();

		SpawnerManager(const SpawnerManager& other) = delete;
		SpawnerManager(SpawnerManager&& other) = delete;
		SpawnerManager& operator=(const SpawnerManager& other) = delete;
		SpawnerManager& operator=(SpawnerManager&& other) = delete;

		~SpawnerManager() = default;

		void CleanUp();

		void Update(float elapsedSec, EnemyManager& manager, const Rectf& camera, const Level& level);
		Spawner* CreateSpawner(const Point2f& screenPos, int count, EnemyType type, const Rectf& spawnBox = Rectf{0,0,0,0});
		void LoadFromFile(const std::string& filepath);

	private:
		SpawnerManager();

		static SpawnerManager* m_pManager;

		std::vector<Spawner*> m_pSpawners;
};

