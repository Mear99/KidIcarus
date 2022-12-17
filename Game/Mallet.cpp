#include "pch.h"
#include "Mallet.h"
#include "SoundManager.h"

Mallet::Mallet(float screenwidth, Tilemap* itemMap)
	: Item(screenwidth, itemMap, 1, 1)
	, m_FallSpeed{ 9.81f }
{
}

void Mallet::Update(float elapsedSec, const Rectf& boundingBox) {
	m_Position.y -= m_FallSpeed * elapsedSec;
	m_Hitbox.bottom = m_Position.y;

	if (m_Position.y + m_Hitbox.height < boundingBox.bottom) {
		Despawn();
	}
}

void Mallet::OnPickUp() {
	SoundManager::GetInstance()->PlayEffect("Collected");
}