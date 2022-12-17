#pragma once
#include "GameObject.h"
#include "Player.h"

class DoorTrigger final : public GameObject
{
	public: 
		explicit DoorTrigger(const Rectf& triggerZone, const Point2f& exitLocation, const std::string locationName);
		bool CheckZone(const Player& player);
		void GetLevel(Point2f& spawnPoint, std::string& locationName);

	private:
		Rectf m_TriggerZone;
		Point2f m_ExitLocation;
		std::string m_LocationName;

};

