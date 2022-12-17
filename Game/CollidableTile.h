#pragma once
#include "Tile.h"
#include "Actor.h"

class CollidableTile : public Tile
{
	public:
		explicit CollidableTile(const Point2f& pos, Tilemap* tilemap, int row, int col);
		virtual bool Collides(const Actor& actor) const override;

		virtual ~CollidableTile() = default;

	protected:
		Rectf m_HitBox;
};

