#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include "Vector2f.h"
#include "Level.h"

// Everything that moves and collides
class Actor : public GameObject
{
	public:
		explicit Actor(const Rectf& hitbox, float screenWidth, Texture* texture, int rows, int cols);
		virtual void Draw() const;
		virtual void Update(float elapsedSec, const Level& level);
		virtual bool CheckCollision(const Rectf& hitbox) const;
		Rectf GetHitbox() const;
		Vector2f GetVelocity() const;

		virtual ~Actor() = default;

	protected:
		void Translate(float deltaX, float deltaY);
		void SetPosition(Point2f pos);
		void WrapX();
		bool IsOnGround(const Level& level);
		void MoveHorizontal(float elapsedSec, const Level& level);
		void MoveVertical(float elapsedSec, const Level& level);

		Sprite m_Sprite;
		Rectf m_Hitbox;
		Vector2f m_Velocity;
		const float m_Gravity;

	private:
		const float m_ScreenWidth;
};

