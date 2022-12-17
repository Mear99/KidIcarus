#include "pch.h"
#include "ItemManager.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Tilemap.h"
#include "Harp.h"
#include "Chalice.h"
#include "Vase.h"
#include <fstream>
#include <sstream>
#include "SoundManager.h"

ItemManager* ItemManager::m_pManager{ nullptr };

ItemManager* ItemManager::GetInstance(float screenwidth) {
	if (!m_pManager) {
		m_pManager = new ItemManager(screenwidth);
	}
	return m_pManager;
}

ItemManager::ItemManager(float screenwidth)
	: m_ScreenWidth{ screenwidth }
	, m_pItemMap{ new Tilemap{ "Resources/Tiles/Itemmap.png", 16 } }
	, m_HarpIsPlaying{ false }
	, m_AccuHarpTime{ 0.0f }
	, m_MaxHarpTime{ 10.0f }
	, m_AccuBeepInterval{ 0 }
	, m_MaxBeepInterval{ 0.5f }
{
}

void ItemManager::CleanUp() {

	for (Item* item : m_pItems) {
		delete item;
	}

	delete m_pItemMap;
	delete m_pManager;
}

void ItemManager::Draw() const {
	for (Item* item : m_pItems) {
		item->Draw();
	}
}

void ItemManager::Update(float elapsedSec, const Rectf& camera) {
	for (Item* item : m_pItems) {
		Mallet* mallet{ dynamic_cast<Mallet*>(item) };
		if (mallet) {
			mallet->Update(elapsedSec, camera);
			if (!IsHarpPlaying()) {
				mallet->Despawn();
			}
		}

	}

	if (IsHarpPlaying()) {
		m_AccuHarpTime += elapsedSec;
		m_AccuBeepInterval += elapsedSec;

		if (m_AccuBeepInterval >= m_MaxBeepInterval) {
			m_AccuBeepInterval -= m_MaxBeepInterval;
			SoundManager::GetInstance()->PlayEffect("Timer");
		}

		if (m_AccuHarpTime >= m_MaxHarpTime) {
			m_AccuHarpTime = 0;
			m_HarpIsPlaying = false;
		}
	}
}

HeartPickup* ItemManager::GetHeart() {
	for (Item* item : m_pItems) {
		HeartPickup* heart{ dynamic_cast<HeartPickup*>(item) };
		if (heart && !heart->IsActive()) {
			return heart;
		}
	}
	m_pItems.push_back(new HeartPickup{ HeartType::small, m_ScreenWidth, m_pItemMap });
	HeartPickup* heart{ dynamic_cast<HeartPickup*>(m_pItems[m_pItems.size() - 1]) };
	return heart;
}

Mallet* ItemManager::GetMallet() {
	for (Item* item : m_pItems) {
		Mallet* mallet{ dynamic_cast<Mallet*>(item) };
		if (mallet && !mallet->IsActive()) {
			return mallet;
		}
	}
	m_pItems.push_back(new Mallet{ m_ScreenWidth, m_pItemMap });
	Mallet* mallet{ dynamic_cast<Mallet*>(m_pItems[m_pItems.size() - 1]) };
	return mallet;
}

void ItemManager::CheckCollisions(const Player& player) {
	for (Item* item : m_pItems) {
		if (item->CheckCollision(player)) {
			item->OnPickUp();
			item->Despawn();
		}
	}
}

void ItemManager::PlayHarp() {
	m_HarpIsPlaying = true;
	m_AccuHarpTime = 0;
}

bool ItemManager::IsHarpPlaying() const {
	return m_HarpIsPlaying;
}

std::vector<Item*> ItemManager::GetItems() {
	return m_pItems;
}


void ItemManager::LoadFromFile(const std::string& filepath) {

	// Remove old items
	for (Item* item : m_pItems) {
		delete item;
	}
	m_pItems.clear();

	std::ifstream inputStream{ filepath };
	if (inputStream) {
		std::string line{};

		// Find the start of the spawners block
		do {
			getline(inputStream, line);
		} while (line.find("<Items>") == std::string::npos && inputStream);

		// Until the end of the spawners block
		getline(inputStream, line, '>');
		std::stringstream block{ line };
		while (block) {
			std::string identifier{};
			char bracket{};
			Point2f point{};
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
				  >> identifier >> point.x >> point.y;

				if (type == "Harp") {
					Harp* harp{ new Harp{m_ScreenWidth, m_pItemMap } };
					harp->Spawn(point);
					m_pItems.push_back(harp);
				}
				else if (type == "Chalice") {
					Chalice* chalice{ new Chalice{m_ScreenWidth, m_pItemMap } };
					chalice->Spawn(point);
					m_pItems.push_back(chalice);
				}
				else if (type == "Vase") {
					Vase* vase{ new Vase{m_ScreenWidth, m_pItemMap } };
					vase->Spawn(point);
					m_pItems.push_back(vase);
				}
			}
		}
	}
}