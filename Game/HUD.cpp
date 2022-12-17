#include "pch.h"
#include "HUD.h"
#include "Tilemap.h"

HUD::HUD() 
	: GameObject(Point2f{20,210})
	, m_Hearts{ 0 }
	, m_Health{ 0 }
	, m_pHUDmap{ new Tilemap{"Resources/Tiles/HUDmap.png", 8.0f} }
{}

HUD::~HUD() {
	delete m_pHUDmap;
}

void HUD::Draw() const {
	DrawScore();
	DrawHealth();
}

void HUD::Update(int health, int hearts) {
	m_Health = health;
	m_Hearts = hearts;
}

void HUD::DrawScore() const {

	float tileSize{ m_pHUDmap->GetTileSize() };
	glPushMatrix();
	glScalef(3, 3, 0);
	glTranslatef(m_Position.x, m_Position.y + tileSize + 1, 0);
	m_pHUDmap->DrawTile(0, 8);

	glPopMatrix();

	int numbers{ 3 };
	if (m_Hearts >= pow(10, numbers)) {
		numbers = int(log10(m_Hearts)) + 1;
	}

	for (int i{0}; i < numbers; ++i) {

		int number{ m_Hearts / int(pow(10,numbers-i - 1))};
		float xPos{ m_Position.x + (1+i)*tileSize };

		glPushMatrix();
		glScalef(3, 3, 0);
		glTranslatef(xPos , m_Position.y + tileSize, 0);
		m_pHUDmap->DrawTile(1, number);

		glPopMatrix();
	}

}

void HUD::DrawHealth() const {
	glPushMatrix();
	glScalef(3, 3, 0);
	glTranslatef(m_Position.x, m_Position.y, 0);
	m_pHUDmap->DrawTile(0, m_Health);

	glPopMatrix();
}