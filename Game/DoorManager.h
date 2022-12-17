#pragma once
#include <vector>
class Player;
class DoorTrigger;

class DoorManager final
{
	public:
		explicit DoorManager() = default;
		DoorManager(const DoorManager& other) = delete;
		DoorManager(DoorManager&& other) = delete;
		DoorManager& operator=(const DoorManager& other) = delete;
		DoorManager& operator=(DoorManager&& other) = delete;

		~DoorManager();

		DoorTrigger* CheckDoors(const Player& player);
		void LoadFromFile(const std::string& filepath);
		void CleanUp();

	private:
		std::vector<DoorTrigger*> m_pDoors;
};

