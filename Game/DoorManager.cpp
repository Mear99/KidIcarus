#include "pch.h"
#include "DoorManager.h"
#include "DoorTrigger.h"
#include <fstream>
#include <sstream>

DoorManager::~DoorManager() {
	CleanUp();
}

DoorTrigger* DoorManager::CheckDoors(const Player& player) {
	for (DoorTrigger* door : m_pDoors) {
		if (door->CheckZone(player)) {
			return door;
		}
	}
	return nullptr;
}

void DoorManager::CleanUp() {
	for (DoorTrigger* door : m_pDoors) {
		delete door;
	}
	m_pDoors.clear();
}

void DoorManager::LoadFromFile(const std::string& filepath) {

	CleanUp();

	std::ifstream inputStream{ filepath };
	if (inputStream) {
		std::string line{};

		// Find the start of the spawners block
		do {
			getline(inputStream, line);
		} while (line.find("<Doors>") == std::string::npos && inputStream);

		// Until the end of the spawners block
		getline(inputStream, line, '>');
		std::stringstream block{ line };
		while (block) {
			std::string identifier{};
			char bracket{};
			Point2f point{};
			std::stringstream s{};
			std::string room;
			Rectf zone{};

			// Get and format spawner
			getline(block, line, ',');
			s << line;
			s >> bracket;
			if (bracket == '{') {
				// Trigger zone
				s >> identifier >> zone.left >> zone.bottom >> zone.width >> zone.height
				// Spawnpoint
				  >> identifier >> point.x >> point.y
				// Room
				  >> identifier >> room;

				m_pDoors.push_back(new DoorTrigger{zone,point, room});
			}
		}
	}
}