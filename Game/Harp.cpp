#include "pch.h"
#include "Harp.h"
#include "ItemManager.h"
#include "SoundManager.h"

Harp::Harp(float screenwidth, Tilemap* itemMap)
	: Item(screenwidth, itemMap, 1, 0)
{
}

void Harp::OnPickUp() {
	ItemManager::GetInstance()->PlayHarp();
	SoundManager::GetInstance()->PlayEffect("Collected");
}