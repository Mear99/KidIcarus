#include "pch.h"
#include "Item.h"
#include "Texture.h"
#include "utils.h"
#include "Tilemap.h"

Item::Item(float screenwidth, Tilemap* itemMap, int row, int col)
	: GameObject(Point2f{ 0,0 })
	, m_IsActive{ false }
	, m_ScreenWidth{ screenwidth }
	, m_pTilemap{ itemMap }
	, m_Row{ row }
	, m_Col{ col }
	, m_Hitbox{ 0,0, itemMap->GetTileSize(), itemMap->GetTileSize() }
{
}

void Item::Draw() const {

	if (m_IsActive) {

		glPushMatrix();

		glTranslatef(m_Position.x, m_Position.y, 0);
		m_pTilemap->DrawTile(m_Row, m_Col);

		if (m_Hitbox.left + m_Hitbox.width > m_ScreenWidth) {

			glTranslatef(-m_ScreenWidth, 0, 0);
			m_pTilemap->DrawTile(m_Row, m_Col);
		}

		glPopMatrix();
	}
}

bool Item::CheckCollision(const Player& player) const {
	if (m_IsActive) {
		return player.CheckCollision(m_Hitbox);
	}
	return false;
}

void Item::Spawn(const Point2f& pos) {

	m_Hitbox.left = pos.x;
	m_Hitbox.bottom = pos.y;
	m_Position = pos;
	m_IsActive = true;
}

void Item::Despawn() {
	m_IsActive = false;
}

bool Item::IsActive() const {
	return m_IsActive;
}