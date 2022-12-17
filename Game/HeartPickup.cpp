#include "pch.h"
#include "HeartPickup.h"
#include "Texture.h"
#include "SoundManager.h"

int HeartPickup::m_Hearts{ 0 };

HeartPickup::HeartPickup(HeartType type, float screenwidth, Tilemap* itemMap)
	: Item(screenwidth, itemMap, 0,int(type))
	, m_Type{ type }
{
}

void HeartPickup::SetSize(HeartType type) {
	m_Type = type;
	m_Col = int(type);
}

void HeartPickup::OnPickUp() {
	switch (m_Type) {
		case HeartType::small:
			m_Hearts += 1;
			break;
		case HeartType::medium:
			m_Hearts += 5;
			break;
		case HeartType::large:
			m_Hearts += 10;
			break;
	}

	SoundManager::GetInstance()->PlayEffect("Collected");
}

int HeartPickup::GetScore() {
	return m_Hearts;
}