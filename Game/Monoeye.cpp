#include "pch.h"
#include "utils.h"
#include "pch.h"
#include "Monoeye.h"
#include "HeartPickup.h"
#include "Vector2f.h"
#include <iostream>

Monoeye::Monoeye(float screenWidth, Texture* texture)
	: Enemy(Rectf{0,0,10,16}, screenWidth, texture, 2, 4)
	, m_Action{ MonoeyeAction::hovering }
	, m_AccuAngle{ 3*utils::g_Pi/2 }
	, m_TrackingPoint{ Point2f{0,0} }
	, m_MaxHoverTime{ 10.0f }
	, m_AccuHoverTime{ 0.0f }
	,  m_Speed{ 150.0f }
	, m_HoverSpeed{ 50.0f }
	, m_TrackingDistance{ 100.0f }
{
	m_HeartDrop = HeartType::medium;
}

void Monoeye::Update(float elapsedSec, const Level& level) {
	if (m_State == EnemyState::alive) {
		switch (m_Action) {
			case MonoeyeAction::hovering:
				Hover(elapsedSec);
				break;

			case MonoeyeAction::lockDive:
				Dive(elapsedSec);
				break;
		}

		MoveVertical(elapsedSec, level);
		MoveHorizontal(elapsedSec, level);
		WrapX();
	}
	// Reset the action when it despawns
	else {
		m_Action = MonoeyeAction::hovering;
	}

	bool flip{ (m_Position.x >= m_TrackingPoint.x) ? false : true };
	m_Sprite.SetFlip(flip);
	m_Sprite.Update(elapsedSec);
}

void Monoeye::Hover(float elapsedSec) {

	m_Velocity.x = m_Speed * cosf(m_AccuAngle);
	m_Velocity.y = m_HoverSpeed * cosf(m_AccuAngle*4);
	m_AccuAngle += 1.25f * elapsedSec;
	m_AccuAngle -= (m_AccuAngle >= utils::g_Pi * 2) ? utils::g_Pi * 2 : 0;

	m_AccuHoverTime += elapsedSec;
	if (m_AccuHoverTime >= m_MaxHoverTime) {
		m_AccuHoverTime = 0;
		m_Action = MonoeyeAction::lockDive;
	}
}

void Monoeye::Dive(float elapsedSec) {

	float angle = atan2(m_TrackingPoint.y - m_Position.y, m_TrackingPoint.x - m_Position.x);

	m_Velocity.x = m_Speed * cos(angle);
	m_Velocity.y = m_Speed * sin(angle);
	m_Action = MonoeyeAction::diving;
}

bool Monoeye::CheckCollision(const Rectf& hitbox) const {
	return false;
}

void Monoeye::SetTrackingPoint(const Point2f& point) {
	if (m_State == EnemyState::alive) {
		if (m_Action == MonoeyeAction::hovering) {
			m_TrackingPoint = point;
			const float distance = utils::GetDistance(m_Position, m_TrackingPoint);
			if (m_TrackingDistance > distance) {
				// Let the monoeye panic upwards
				Translate(0, m_TrackingDistance - distance);
			}
		}
	}
}
