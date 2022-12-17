#include "pch.h"
#include "Tilemap.h"
#include "Texture.h"

Tilemap::Tilemap(std::string path, float tileSize) 
	: m_pTilemap{ new Texture{ path } }
	, m_TileSize{ tileSize }
{
	m_Rows = int(m_pTilemap->GetHeight() / m_TileSize);
	m_Cols = int(m_pTilemap->GetWidth() / m_TileSize);
}

Tilemap::~Tilemap() {
	delete m_pTilemap;
}

void Tilemap::DrawTile(int row, int col) const{
	Rectf srcRect{};
	srcRect.left = col * m_TileSize;
	srcRect.bottom = (1 + row) * m_TileSize;
	srcRect.width = m_TileSize;
	srcRect.height = m_TileSize;

	m_pTilemap->Draw(Point2f{ 0,0 }, srcRect);
}

float Tilemap::GetTileSize() const{
	return m_TileSize;
}
