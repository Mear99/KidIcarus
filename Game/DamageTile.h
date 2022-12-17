#pragma once
#include "CollidableTile.h"
#include "Player.h"

class DamageTile final : public Tile
{
	public:
		explicit DamageTile(const Point2f& pos, Tilemap* tilemap);
		void Update(float elapsedSec);
		bool CheckCollision(const Player& player);
		int GetDamage() const;

	private:
		Rectf m_HitBox;

		int m_Damage;
		float m_AccuCooldown;
		float m_MaxCooldown;
};

