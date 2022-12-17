#include "pch.h"
#include "Chalice.h"
#include "ItemManager.h"
#include "SoundManager.h"

Chalice::Chalice(float screenwidth, Tilemap* itemMap)
	: Item(screenwidth, itemMap, 1, 2)
{
}

void Chalice::OnPickUp() {
	Player::Heal();
	SoundManager::GetInstance()->PlayEffect("Collected");
}