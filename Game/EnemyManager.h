#pragma once
#include "Bullet.h"
#include <vector>
#include "Level.h"
#include "Player.h"
class Enemy;
class Spawner;

enum class EnemyType { Shemum, Monoeye, Reaper, Reapette, Nettler, McGoo, Specknose };

class EnemyManager final
{
	public:
		explicit EnemyManager(float screenwidth);

		EnemyManager(const EnemyManager& other) = delete;
		EnemyManager(EnemyManager&& other) = delete;
		EnemyManager& operator=(const EnemyManager& other) = delete;
		EnemyManager& operator=(EnemyManager&& other) = delete;

		~EnemyManager();

		void Update(float elapsedSec, const Level& level, const Rectf& camera, const Player& player);
		void Draw() const;
		Enemy* CreateEnemy(EnemyType type);
		int CheckHit(const Player& player);
		std::vector<Enemy*> GetEnemies();
		void LoadFromFile(const std::string& filepath);
		void CleanUp();

	private:
		std::vector<Enemy*> m_pEnemies;
		std::vector<Texture*> m_pEnemyTextures;
		float m_ScreenWidth;
};

