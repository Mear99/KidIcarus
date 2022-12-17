#include "pch.h"
#include "SpawnerManager.h"
#include "Spawner.h"
#include "Level.h"
#include <fstream>
#include <sstream>

SpawnerManager* SpawnerManager::m_pManager{ nullptr };

SpawnerManager* SpawnerManager::GetInstance() {
	if (!m_pManager) {
		m_pManager = new SpawnerManager();
	}
	return m_pManager;
}

SpawnerManager::SpawnerManager() {}

void SpawnerManager::CleanUp() {
	for (Spawner* spawner : m_pSpawners) {
		delete spawner;
	}

	delete m_pManager;
}

void SpawnerManager::Update(float elapsedSec, EnemyManager& manager, const Rectf& camera, const Level& level) {
	for (Spawner* spawner : m_pSpawners) {
		spawner->Update(elapsedSec, manager, camera, level);
	}
}

Spawner* SpawnerManager::CreateSpawner(const Point2f& screenPos, int count, EnemyType type, const Rectf& spawnBox) {
	m_pSpawners.push_back(new Spawner(screenPos, count, type, spawnBox));
	return m_pSpawners[m_pSpawners.size() - 1];
}

void SpawnerManager::LoadFromFile(const std::string& filepath) {

	// Remove old spawners
	for (Spawner* spawner : m_pSpawners) {
		delete spawner;
	}
	m_pSpawners.clear();

	std::ifstream inputStream{ filepath };
	if (inputStream) {
		std::string line{};

		// Find the start of the spawners block
		do{
			getline(inputStream, line);
		} while (line.find("<Spawners>") == std::string::npos && inputStream);

		// Until the end of the spawners block
		getline(inputStream, line, '>');
		std::stringstream block{ line };
		while (block) {
			std::string identifier{};
			char bracket{};
			Point2f point{};
			int amount{};
			EnemyType type{};
			Rectf bounds{};
			std::stringstream s{};
			std::string typeString;

			// Get and format spawner
			getline(block, line, ',');
			s << line;
			s >> bracket;
			if (bracket == '{') {
				// Point
				s >> identifier >> point.x >> point.y
					//Amount
					>> identifier >> amount
					// Type
					>> identifier >> typeString
					// Bounds
					>> identifier >> bounds.left >> bounds.bottom >> bounds.width >> bounds.height;

				if (typeString.find("Shemum") != std::string::npos) {
					type = EnemyType::Shemum;
				}
				else if (typeString.find("Monoeye") != std::string::npos) {
					type = EnemyType::Monoeye;
				}
				else if (typeString.find("Nettler") != std::string::npos) {
					type = EnemyType::Nettler;
				}
				else if (typeString.find("McGoo") != std::string::npos) {
					type = EnemyType::McGoo;
				}

				m_pSpawners.push_back(new Spawner(point, amount, type, bounds));
			}
		}
	}
}