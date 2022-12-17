#include "pch.h"
#include "utils.h"
#include "pch.h"
#include "Reapette.h"
#include "HeartPickup.h"

Reapette::Reapette(float screenWidth, Texture* texture)
	: Enemy(Rectf{ 0,0,8,16 }, screenWidth, texture, 2, 4, 1, 2)
	, m_Action{ ReapetteAction::hovering }
	, m_AccuAngle{ 3 * utils::g_Pi / 2 }
	, m_TrackingPoint{ Point2f{0,0} }
	, m_MaxHoverTime{ 10.0f }
	, m_AccuHoverTime{ 0.0f }
	, m_Speed{ 100.0f }
{
}

void Reapette::Update(float elapsedSec, const Level& level) {
	if (m_State == EnemyState::alive) {
		switch (m_Action) {
			case ReapetteAction::hovering:
				Hover(elapsedSec);
				break;

			case ReapetteAction::lockDive:
				Dive(elapsedSec);
				break;
			case ReapetteAction::diving:
				CheckDive();
		}

		MoveVertical(elapsedSec, level);
		MoveHorizontal(elapsedSec, level);
		WrapX();
	}
	// Reset the action when it despawns
	else {
		m_Action = ReapetteAction::hovering;
	}

	bool flip{ (m_Position.x >= m_TrackingPoint.x) ? false : true };
	m_Sprite.SetFlip(flip);
	m_Sprite.Update(elapsedSec);
}

void Reapette::Hover(float elapsedSec) {

	m_Velocity.x = -m_Speed * sinf(m_AccuAngle);
	m_Velocity.y = -m_Speed * cosf(m_AccuAngle);
	m_AccuAngle += 1.25f * elapsedSec;
	m_AccuAngle -= (m_AccuAngle >= utils::g_Pi * 2) ? utils::g_Pi * 2 : 0;

	m_AccuHoverTime += elapsedSec;
	if (m_AccuHoverTime >= m_MaxHoverTime) {
		m_AccuHoverTime = 0;
		m_Action = ReapetteAction::lockDive;
	}
}

void Reapette::Dive(float elapsedSec) {

	float angle = atan2f(m_TrackingPoint.y - m_Position.y, m_TrackingPoint.x - m_Position.x);

	m_Velocity.x = m_Speed * cosf(angle);
	m_Velocity.y = m_Speed * sinf(angle);
	m_Action = ReapetteAction::diving;
}

bool Reapette::CheckCollision(const Rectf& hitbox) const {
	return false;
}

void Reapette::SetTrackingPoint(const Point2f& point) {
	if (m_State == EnemyState::alive) {
		if (m_Action == ReapetteAction::hovering) {
			m_TrackingPoint = point;
			const float distance = utils::GetDistance(m_Position, m_TrackingPoint);
		}
	}
}

void Reapette::CheckDive() {
	if (!(m_TrackingPoint.x > m_Position.x && m_Velocity.x > 0) &&
		!(m_TrackingPoint.x < m_Position.x && m_Velocity.x < 0)) {
		m_Action = ReapetteAction::hovering;
	}
}
