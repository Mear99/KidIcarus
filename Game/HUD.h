#pragma once
#include "GameObject.h"
class Tilemap;

class HUD final : public GameObject
{
	public:
		explicit HUD();

		HUD(const HUD& other) = delete;
		HUD(HUD&& other) = delete;
		HUD& operator=(const HUD& other) = delete;
		HUD& operator=(HUD&& other) = delete;

		~HUD();

		void Update(int health, int hearts);
		void Draw() const;

	private:
		int m_Health;
		int m_Hearts;
		Tilemap* m_pHUDmap;

		void DrawScore() const;
		void DrawHealth() const;

};

