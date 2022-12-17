#include "pch.h"
#include "Specknose.h"

Specknose::Specknose(float screenWidth, Texture* texture)
	: Enemy(Rectf{ 0,0,16,16 }, screenWidth, texture, 2, 4, 1, 2)
	, m_PatrolLeft{}
	, m_PatrolRight{}
	, m_Speed{ 50.0f }
{
	m_Velocity = Vector2f{ m_Speed, m_Speed };
	m_HeartDrop = HeartType::large;
}

void Specknose::Update(float elapsedSec, const Level& level) {
	if (m_State == EnemyState::alive) {
		if ((m_Velocity.x < 0 && m_Position.x < m_PatrolLeft.x)
			|| (m_Velocity.x > 0 && m_Position.x > m_PatrolRight.x)) {
			m_Velocity.x *= -1;
		}

		if ((m_Velocity.y < 0 && m_Position.y < m_PatrolLeft.y)
			|| (m_Velocity.y > 0 && m_Position.y > m_PatrolRight.y)) {
			m_Velocity.y *= -1;
		}
		MoveHorizontal(elapsedSec, level);
		MoveVertical(elapsedSec, level);
	}

	UpdateHitPeriod(elapsedSec);
	m_Sprite.SetVariant(m_IsHit);
	m_Sprite.Update(elapsedSec);
}

void Specknose::SetPatrolPoints(const Point2f& left, const Point2f& right) {

	m_PatrolLeft = left;
	m_PatrolRight = right;
}

bool Specknose::CheckCollision(const Rectf& hitbox) const {
	return false;
}
