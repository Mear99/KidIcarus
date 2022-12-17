#include "pch.h"
#include "DoorTrigger.h"

DoorTrigger::DoorTrigger(const Rectf& triggerZone, const Point2f& exitLocation, const std::string locationName)
	: GameObject(Point2f{ triggerZone.left, triggerZone.bottom })
	, m_TriggerZone{ triggerZone }
	, m_ExitLocation{ exitLocation }
	, m_LocationName{ locationName }
{}

bool DoorTrigger::CheckZone(const Player& player) {
	return player.CheckCollision(m_TriggerZone);
}

void DoorTrigger::GetLevel(Point2f& spawnPoint, std::string& locationName) {
	spawnPoint = m_ExitLocation;
	locationName = m_LocationName;
}