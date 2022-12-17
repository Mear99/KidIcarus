#pragma once
#include "GameObject.h"
#include "Player.h"
class Texture;


class Item : public GameObject
{
	public:
		explicit Item(float screenwidth, Tilemap* itemMap, int row, int col);
		void Draw() const;
		virtual bool CheckCollision(const Player& player) const;
		void Spawn(const Point2f& pos);
		void Despawn();
		bool IsActive() const;
		virtual void OnPickUp() = 0;

		virtual ~Item() = default;

	protected:
		int m_Col;
		Rectf m_Hitbox;
		bool m_IsActive;

	private:
		float m_ScreenWidth;

		Tilemap* m_pTilemap;
		int m_Row;

};

