#include "pch.h"
#include "Nettler.h"
#include "Vector2f.h"


Nettler::Nettler(float screenWidth, Texture* texture)
	: Enemy(Rectf{ 0,0,10,16 }, screenWidth, texture, 3, 4)
	, m_Speed{ 30.0f }
{
	m_Velocity = Vector2f{ m_Speed,0 };
	m_HeartDrop = HeartType::medium;
}

void Nettler::Update(float elapsedSec, const Level& level) {
	if (m_State == EnemyState::alive) {
		if (IsOnGround(level)) {
			MoveHorizontal(elapsedSec, level);
			WrapX();

			// Track player
			if (abs(m_TrackingPoint.y - m_Hitbox.bottom) < m_Hitbox.height) {
				m_Velocity.x = (m_Position.x < m_TrackingPoint.x) ? m_Speed : -m_Speed;
			}
			// Neutral behaviour
			else if (IsAtWall(level)) {
				m_Velocity.x *= -1;
			}
		}
		else {
			m_Velocity.y -= m_Gravity * elapsedSec;
			MoveVertical(elapsedSec, level);

		}
	}
	bool flip{ (m_Velocity.x < 0) ? true : false };
	m_Sprite.SetFlip(flip);
	m_Sprite.Update(elapsedSec);
}

bool Nettler::IsAtWall(const Level& level) {

	const float delta{ (m_Velocity.x < 0) ? -1.0f : 1.0f };

	Translate(delta, 0);
	const bool isAtWall{ level.Collides(*this) };
	Translate(-delta, 0);
	return isAtWall;
}

void Nettler::SetTrackingPoint(const Point2f& point) {
	if (m_State == EnemyState::alive) {
		m_TrackingPoint = point;
	}
}