#pragma once
#include "CollidableTile.h"

class Platform final : public CollidableTile
{
	public:
		explicit Platform(const Point2f& pos, Tilemap* tilemap);
		virtual bool Collides(const Actor& actor) const override;
};

