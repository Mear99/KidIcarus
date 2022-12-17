#include "pch.h"
#include "CollidableTile.h"
#include "Tilemap.h"

CollidableTile::CollidableTile(const Point2f& pos, Tilemap* tilemap, int row, int col)
	: Tile(pos, tilemap, row, col)
	, m_HitBox{ Rectf{ pos.x, pos.y, tilemap->GetTileSize(), tilemap->GetTileSize()}}
{
}

bool CollidableTile::Collides(const Actor& actor) const {

	return actor.CheckCollision(m_HitBox);
}