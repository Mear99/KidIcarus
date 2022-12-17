#include "pch.h"
#include "Vase.h"
#include "ItemManager.h"

Vase::Vase(float screenwidth, Tilemap* itemMap)
	: Item(screenwidth, itemMap, 2, 0)
{
}

bool Vase::CheckCollision(const Player& player) const {
	return false;
}
bool Vase::CheckCollision(const Bullet& bullet) const {
	if (m_IsActive && bullet.GetType() == BulletType::arrow) {
		return bullet.CheckCollision(m_Hitbox);

	}
	return false;
}

void Vase::OnPickUp() {
	int chance{ rand() % 10 };

	HeartPickup* heart{ ItemManager::GetInstance()->GetHeart() };

	switch (chance) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			heart->SetSize(HeartType::small);
			heart->Spawn(m_Position);
			break;

		case 5:
		case 6:
		case 7:
			heart->SetSize(HeartType::medium);
			heart->Spawn(m_Position);
			break;

		case 8:
			heart->SetSize(HeartType::large);
			heart->Spawn(m_Position);
			break;
	}
}