#include "pch.h"
#include "Tile.h"
#include "Tilemap.h"

Tile::Tile(const Point2f& pos, Tilemap* tilemap, int row, int col)
	: GameObject(pos)
	, m_pTilemap{ tilemap }
	, m_Row{ row }
	, m_Col{ col }
{
}

void Tile::Draw() const {
	glPushMatrix();

	glTranslatef(m_Position.x, m_Position.y, 0);
	m_pTilemap->DrawTile(m_Row, m_Col);

	glPopMatrix();
}

bool Tile::Collides(const Actor& actor) const {
	return false;
}
