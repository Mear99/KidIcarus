#include "pch.h"
#include "EnemyManager.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Shemum.h"
#include "Monoeye.h"
#include "Reaper.h"
#include "Reapette.h"
#include "Nettler.h"
#include "McGoo.h"
#include "Specknose.h"
#include "Texture.h"
#include "ItemManager.h"
#include "utils.h"
#include <fstream>
#include <sstream>

EnemyManager::EnemyManager(float screenwidth)
	: m_ScreenWidth{ screenwidth }
{
	m_pEnemyTextures.push_back(new Texture{ "Resources/Sprites/Shemum.png" });
	m_pEnemyTextures.push_back(new Texture{ "Resources/Sprites/Monoeye.png" });
	m_pEnemyTextures.push_back(new Texture{ "Resources/Sprites/reaper.png" });
	m_pEnemyTextures.push_back(new Texture{ "Resources/Sprites/reaperette.png" });
	m_pEnemyTextures.push_back(new Texture{ "Resources/Sprites/nettler.png" });
	m_pEnemyTextures.push_back(new Texture{ "Resources/Sprites/McGoo.png" });
	m_pEnemyTextures.push_back(new Texture{ "Resources/Sprites/Specknose.png" });
}

EnemyManager::~EnemyManager() {
	CleanUp();
	for (Texture* texture : m_pEnemyTextures) {
		delete texture;
	}
}

void EnemyManager::CleanUp() {
	for (Enemy* enemy : m_pEnemies) {
		delete enemy;
	}
	m_pEnemies.clear();
}

void EnemyManager::Update(float elapsedSec, const Level& level, const Rectf& camera, const Player& player) {

	ItemManager* itemManager{ ItemManager::GetInstance() };
	for (Enemy* enemy : m_pEnemies) {
		if (itemManager->IsHarpPlaying() && utils::IsOverlapping(camera, enemy->GetHitbox())) {
			enemy->Transmutate();
		}

		enemy->Update(elapsedSec, level);
		enemy->CheckBounds(camera);

		Monoeye* monoeye{ dynamic_cast<Monoeye*>(enemy) };
		if (monoeye) {
			Rectf playerHitbox{ player.GetHitbox() };
			monoeye->SetTrackingPoint(Point2f{ playerHitbox.left, playerHitbox.bottom });
		}

		Reapette* reapette{ dynamic_cast<Reapette*>(enemy) };
		if (reapette) {
			Rectf playerHitbox{ player.GetHitbox() };
			reapette->SetTrackingPoint(Point2f{ playerHitbox.left, playerHitbox.bottom });
		}

		Nettler* nettler{ dynamic_cast<Nettler*>(enemy) };
		if (nettler) {
			Rectf playerHitbox{ player.GetHitbox() };
			nettler->SetTrackingPoint(Point2f{ playerHitbox.left, playerHitbox.bottom });
		}

		McGoo* mcGoo{ dynamic_cast<McGoo*>(enemy) };
		if (mcGoo) {
			Rectf playerHitbox{ player.GetHitbox() };
			mcGoo->SetTrackingPoint(Point2f{ playerHitbox.left, playerHitbox.bottom });
		}

		Reaper* reaper{ dynamic_cast<Reaper*>(enemy) };
		if (reaper) {
			reaper->CheckSight(player);
		}
	}
}

void EnemyManager::Draw() const {
	for (Enemy* enemy : m_pEnemies) {
		enemy->Draw();
	}
}

Enemy* EnemyManager::CreateEnemy( EnemyType type) {

	switch (type) {
		case EnemyType::Shemum:
			m_pEnemies.push_back(new Shemum(m_ScreenWidth, m_pEnemyTextures[int(type)]));
			break;

		case EnemyType::Reapette:
			m_pEnemies.push_back(new Reapette(m_ScreenWidth, m_pEnemyTextures[int(type)]));
			break;

		case EnemyType::Monoeye:
			m_pEnemies.push_back(new Monoeye(m_ScreenWidth, m_pEnemyTextures[int(type)]));
			break;

		case EnemyType::Nettler:
			m_pEnemies.push_back(new Nettler(m_ScreenWidth, m_pEnemyTextures[int(type)]));
			break;

		case EnemyType::McGoo:
			m_pEnemies.push_back(new McGoo(m_ScreenWidth, m_pEnemyTextures[int(type)]));
			break;
	}
	return m_pEnemies[m_pEnemies.size()-1];
}

int EnemyManager::CheckHit(const Player& player) {
	for (Enemy* enemy : m_pEnemies) {
		if (enemy->CheckHit(player)) {
			return enemy->GetDamage();
		}
	}
	return 0;
}

std::vector<Enemy*> EnemyManager::GetEnemies() {
	return m_pEnemies;
}

void EnemyManager::LoadFromFile(const std::string& filepath) {

	CleanUp();

	std::ifstream inputStream{ filepath };
	if (inputStream) {
		std::string line{};

		// Find the start of the spawners block
		do {
			getline(inputStream, line);
		} while (line.find("<Enemies>") == std::string::npos && inputStream);

		// Until the end of the spawners block
		getline(inputStream, line, '>');
		std::stringstream block{ line };
		while (block) {
			std::string identifier{};
			char bracket{};
			Point2f point{}, patrolPoint1{}, patrolPoint2{};
			std::stringstream s{};
			std::string type;

			// Get and format spawner
			getline(block, line, ',');
			s << line;
			s >> bracket;
			if (bracket == '{') {
				// Type
				s >> identifier >> type
				// Spawnpoint
				  >> identifier >> point.x >> point.y
				// Patrol points
				  >> identifier >> patrolPoint1.x >> patrolPoint1.y >> patrolPoint2.x >> patrolPoint2.y;

				if (type == "Reaper") {
					Reaper* reaper{ new Reaper(m_ScreenWidth, m_pEnemyTextures[int(EnemyType::Reaper)]) };
					reaper->Spawn(point, nullptr);
					reaper->SetPatrolPoints(patrolPoint1, patrolPoint2);
					m_pEnemies.push_back(reaper);
				}
				else if (type == "Specknose") {
					Specknose* specknose{ new Specknose(m_ScreenWidth, m_pEnemyTextures[int(EnemyType::Specknose)]) };
					specknose->Spawn(point, nullptr);
					specknose->SetPatrolPoints(patrolPoint1, patrolPoint2);
					m_pEnemies.push_back(specknose);
				}
			}
		}
	}
}