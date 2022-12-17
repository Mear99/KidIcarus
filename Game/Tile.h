#pragma once
#include "GameObject.h"
#include "Actor.h"
class Tilemap;

class Tile : public GameObject
{
	public:
		explicit Tile(const Point2f& pos, Tilemap* tilemap, int row, int col);
		void Draw() const;
		virtual bool Collides(const Actor& actor) const;

		virtual ~Tile() = default;

	private:
		Tilemap* m_pTilemap;
		int m_Row;
		int m_Col;
};

