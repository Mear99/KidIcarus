#include "pch.h"
#include "Platform.h"
#include "Player.h"

Platform::Platform(const Point2f& pos, Tilemap* tilemap) 
	: CollidableTile(pos, tilemap, 0, 2)
{
}

bool Platform::Collides(const Actor& actor) const {

	const Player* pPlayer{ dynamic_cast<const Player*>(&actor) };
	if (pPlayer) {
		if (pPlayer->IsCrouching()) {
			return false;
		}
	}

	if (CollidableTile::Collides(actor)) {
		const Rectf hitbox{ actor.GetHitbox()};

		return (actor.GetVelocity().y <= 0) && (hitbox.bottom > m_HitBox.bottom + m_HitBox.height - 2);
	}
	return false;
}