#include "pch.h"
#include "Reaper.h"
#include "SpawnerManager.h"
#include "SoundManager.h"

Reaper::Reaper(float screenWidth, Texture* texture)
	: Enemy(Rectf{ 0,0,16,24 }, screenWidth, texture, 3, 4, 10, 2)
	, m_Action{ ReaperAction::patrol }
	, m_PatrolLeft{}
	, m_PatrolRight{}
	, m_AccuPanicTime{ 0.0f }
	, m_MaxPanicTime{ 7.0f }
	, m_AccuLookbackTime { 0.0f }
	, m_MaxLookbackTime{ 7.0f }
	, m_LookbackDuration{ 0.7f }
	, m_pReapetteSpawner{ nullptr }
	, m_Speed{ 30.0f }
	, m_PanicSpeed{ 90.0f }
	, m_TrackingDistance{ 50.0f }
{
	m_Velocity = Vector2f{ m_Speed, 0 };
	m_HeartDrop = HeartType::large;
}

void Reaper::Update(float elapsedSec, const Level& level) {
	if (m_State == EnemyState::alive) {
		switch (m_Action) {
			case ReaperAction::patrol:
				Patrol(elapsedSec);
				CheckLookback(elapsedSec);
				break;

			case ReaperAction::panic:
				Panic(elapsedSec);
				break;

			case ReaperAction::lookback:
				Lookback(elapsedSec);
				break;
		}
		if (m_Action != ReaperAction::lookback) {
			if (IsOnGround(level)) {
				MoveHorizontal(elapsedSec, level);
				WrapX();
			}
			else {
				m_Velocity.y -= m_Gravity * elapsedSec;
				MoveVertical(elapsedSec, level);

			}
		}

	}
	// Reset the action when it despawns
	else {
		m_Action = ReaperAction::patrol;
	}

	
	bool flip{ (m_Velocity.x < 0) ? false : true };
	m_Sprite.SetFlip(flip);
	UpdateHitPeriod(elapsedSec);
	m_Sprite.SetVariant(m_IsHit);
	m_Sprite.Update(elapsedSec);
}

void Reaper::SetPatrolPoints(const Point2f& left, const Point2f& right) {
	m_PatrolLeft = left;
	m_PatrolRight = right;
}

void Reaper::Patrol(float elapsedSec) {
	if ( (m_Velocity.x < 0 && m_Position.x < m_PatrolLeft.x) 
		|| (m_Velocity.x > 0 && m_Position.x > m_PatrolRight.x) ) {
		m_Velocity.x *= -1;
	}
}

void Reaper::Panic(float elapsedSec) {
	if ((m_Velocity.x < 0 && m_Position.x < m_PatrolLeft.x)
		|| (m_Velocity.x > 0 && m_Position.x > m_PatrolRight.x)) {
		m_Velocity.x *= -1;
	}

	m_AccuPanicTime += elapsedSec;
	if (m_AccuPanicTime >= m_MaxPanicTime) {
		StopPanic();
	}
}

void Reaper::CheckSight(const Player& player) {

	if (m_Action != ReaperAction::panic && m_State == EnemyState::alive) {
		Rectf sight{ m_Hitbox };
		sight.left += (m_Velocity.x > 0) ? m_Hitbox.width : -m_Hitbox.width;

		if (player.CheckCollision(sight)) {
			StartPanic();
		}
	}
	else if (m_State == EnemyState::dead && m_pReapetteSpawner != nullptr) {
		m_pReapetteSpawner->DeactivateSpawner();
	}
}

void Reaper::StartPanic() {
	m_Action = ReaperAction::panic;
	m_Velocity.x = (m_Velocity.x > 0) ? m_PanicSpeed : -m_PanicSpeed;
	m_Sprite.SetRow(2);
	m_AccuPanicTime = 0;

	if (!m_pReapetteSpawner) {
		m_pReapetteSpawner = SpawnerManager::GetInstance()->CreateSpawner(Point2f{ 120.0f , m_Position.y + 120.0f }, 4, EnemyType::Reapette);
	}
	m_pReapetteSpawner->ActivateSpawner();

	SoundManager::GetInstance()->PlayMusic("Reaper");
}

void Reaper::StopPanic() {
	m_AccuPanicTime = 0;
	m_Action = ReaperAction::patrol;
	m_Sprite.SetRow(0);
	m_Velocity.x = (m_Velocity.x > 0) ? m_Speed : -m_Speed;
	m_pReapetteSpawner->DeactivateSpawner();

	SoundManager::GetInstance()->PlayMusic("Underworld");
}

void Reaper::CheckLookback(float elapsedSec) {
	m_AccuLookbackTime += elapsedSec;
	if (m_AccuLookbackTime >= m_MaxLookbackTime) {
		m_Action = ReaperAction::lookback;
		m_Velocity.x *= -1;
		m_AccuLookbackTime = 0;
	}
}

void Reaper::Lookback(float elapsedSec) {
	m_AccuLookbackTime += elapsedSec;
	if (m_AccuLookbackTime >= m_LookbackDuration) {
		m_Action = ReaperAction::patrol;
		m_Velocity.x *= -1;
		m_AccuLookbackTime = 0;
	}
}

void Reaper::Despawn() {
	SoundManager::GetInstance()->PlayMusic("Underworld");
	Enemy::Despawn();
}
