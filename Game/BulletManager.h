#pragma once
#include "Bullet.h"
#include "Item.h"
#include <vector>
#include "Level.h"
#include "Player.h"
class Enemy;

class BulletManager final
{
	public:
		static BulletManager* GetInstance(float screenwidth = 0);

		BulletManager(const BulletManager& other) = delete;
		BulletManager(BulletManager&& other) = delete;
		BulletManager& operator=(const BulletManager& other) = delete;
		BulletManager& operator=(BulletManager&& other) = delete;

		~BulletManager() = default;

		void CleanUp();

		void Update(float elapsedSec, const Level& level);
		void Draw() const;
		bool CheckCollisions(std::vector<Enemy*> enemies);
		bool CheckCollisions(std::vector<Item*> items);
		bool CheckCollisions(const Level& level);
		int CheckCollisions(const Player& player);
		Bullet* GetBullet(BulletType type);

	private:
		BulletManager(float screenwidth);

		static BulletManager* m_pManager;

		std::vector<Bullet*> m_pBullets;
		std::vector<Texture*> m_pBulletTextures;
		float m_ScreenWidth;
};

