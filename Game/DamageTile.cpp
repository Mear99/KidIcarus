#include "pch.h"
#include "DamageTile.h"
#include "Player.h"
#include "Tilemap.h"

DamageTile::DamageTile(const Point2f& pos, Tilemap* tilemap)
	: Tile(pos, tilemap, 0, 5)
	, m_HitBox{ Rectf{ pos.x, pos.y, tilemap->GetTileSize(), tilemap->GetTileSize()} }
	, m_Damage{ 1 }
	, m_AccuCooldown{ 0 }
	, m_MaxCooldown{ 0.5f }
{
}

void DamageTile::Update(float elapsedSec) {
	m_AccuCooldown += elapsedSec;
}

bool DamageTile::CheckCollision(const Player& player){
	if (player.CheckCollision(m_HitBox) && m_AccuCooldown > m_MaxCooldown) {
		m_AccuCooldown = 0;
		return true;
	}
	return false;
}

int DamageTile::GetDamage() const{
	return m_Damage;
}

