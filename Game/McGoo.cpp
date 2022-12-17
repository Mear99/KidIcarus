#include "pch.h"
#include "McGoo.h"
#include "Vector2f.h"
#include "BulletManager.h"


McGoo::McGoo(float screenWidth, Texture* texture)
	: Enemy(Rectf{ 0,0,10,16 }, screenWidth, texture, 4, 4)
	, m_Action{ McGooAction::pickLocation }
	, m_AccuUncoverTime{ 0 }
	, m_MaxUncoverTime{ 2.0f }
	, m_BulletSpeed{ 30.0f }
{
	m_HeartDrop = HeartType::medium;
}

void McGoo::Update(float elapsedSec, const Level& level) {
	if (m_State == EnemyState::alive) {
		switch (m_Action) {
			case McGooAction::pickLocation:
				SetPosition(m_TrackingPoint);
				if (m_TrackingPoint.x + m_TrackingPoint.y != 0) {
					m_Action = McGooAction::uncover;
				}
				break;

			case McGooAction::uncover:

				while (!IsOnGround(level)) {
					m_Velocity.y = -17 / elapsedSec;
					MoveVertical(elapsedSec, level);
				}


				m_AccuUncoverTime += elapsedSec;
				if (m_AccuUncoverTime > m_MaxUncoverTime) {
					m_AccuUncoverTime = 0;
					m_Action = McGooAction::shoot;
				}

				if (m_AccuUncoverTime < (m_MaxUncoverTime / 2)) {
					m_Sprite.SetRow(0);
				}
				else {
					m_Sprite.SetRow(2);
				}

				break;

			case McGooAction::shoot:
				ShootBullet();
				m_Sprite.SetRow(3);
				m_Action = McGooAction::shooting;
				break;

			case McGooAction::shooting:
				m_AccuUncoverTime += elapsedSec;
				if (m_AccuUncoverTime > m_MaxUncoverTime) {
					m_AccuUncoverTime = 0;
					m_Action = McGooAction::pickLocation;
					Despawn();
				}
				break;
		}
	}
	m_Sprite.Update(elapsedSec);
}

void McGoo::SetTrackingPoint(const Point2f& point) {
	m_TrackingPoint = point;
}

bool McGoo::CheckHit(const Player& player) const {
	if (m_Action == McGooAction::shoot || m_Action == McGooAction::shooting) {
		return Enemy::CheckHit(player);
	}
	return false;
}

void McGoo::BulletDespawned() {

}

void McGoo::ShootBullet() {
	Bullet* bullet = BulletManager::GetInstance()->GetBullet(BulletType::bullet);

	Point2f pos{ };
	pos.x = m_Position.x + m_Hitbox.width / 2;
	pos.y = m_Position.y + m_Hitbox.height / 2;
	Vector2f velocity{};
	int direction{ (m_Position.x < m_TrackingPoint.x) ? 1 : -1 };

	velocity.x = m_BulletSpeed * 2 * direction;

	bullet->Shoot(pos, velocity, this);
}
