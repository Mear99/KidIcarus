#include "pch.h"
#include "Enemy.h"
#include "utils.h"
#include "Spawner.h"
#include "ItemManager.h"
#include "HeartPickup.h"
#include "SoundManager.h"

Enemy::Enemy(const Rectf& hitbox, float screenWidth, Texture* texture, int rows, int cols, int health, int damage)
	: Actor(hitbox, screenWidth, texture, rows, cols)
	, m_State{ EnemyState::idle }
	, m_Owner{ nullptr }
	, m_Health{ health }
	, m_Damage{ damage }
	, m_HeartDrop{ HeartType::small }
	, m_IsHit{ false }
	, m_MaxHitPeriod{ 2.0f }
	, m_AccuHitPeriod{ 0 }
{
}

void Enemy::Draw() const {
	if (m_State != EnemyState::idle) {
		Actor::Draw();
	}
}

void Enemy::Update(float elapsedSec, const Level& level) {
	switch (m_State) {
		case EnemyState::alive:
			UpdateHitPeriod(elapsedSec);
			m_Sprite.SetVariant(m_IsHit);
			Actor::Update(elapsedSec, level);
			break;
		case EnemyState::dead:
			m_Sprite.Update(elapsedSec);
			break;
	}
}

void Enemy::Spawn(const Point2f& pos, Spawner* owner) {
	SetPosition(pos);
	m_State = EnemyState::alive;
	m_Owner = owner;

	m_Sprite.SetLoop(true);
	m_Sprite.SetRow(0);
}

bool Enemy::CheckHit(const Bullet& bullet) const {
	return(m_State == EnemyState::alive && bullet.Collides(*this));
		
}

bool Enemy::CheckHit(const Player& player) const {
	if (m_State == EnemyState::alive) {

		return player.CheckCollision(m_Hitbox);
	}
	return false;
}

bool Enemy::IsActive() {
	return m_State != EnemyState::idle;
}

void Enemy::TakeDamage(int damage) {

	m_Health -= damage;

	if (m_Health <= 0) {
		m_IsHit = false;
		HeartPickup* pickup = ItemManager::GetInstance()->GetHeart();
		pickup->SetSize(m_HeartDrop);
		pickup->Spawn(m_Position);
		Despawn();

		SoundManager::GetInstance()->PlayEffect("EnemyKilled");
	}
	else {
		m_IsHit = true;
		m_AccuHitPeriod = 0.0f;
	}
}

void Enemy::Despawn() {
	m_State = EnemyState::dead;
	m_Sprite.SetLoop(false);
	m_Sprite.SetRow(int(m_State));

	if (m_Owner) {
		m_Owner->EnemyDespawned();
	}
}

void Enemy::CheckBounds(const Rectf& bounds) {
	if (m_State == EnemyState::alive) {
		if (m_Hitbox.bottom + m_Hitbox.height < bounds.bottom) {
			Despawn();
		}
	}
}

void Enemy::Transmutate() {
	if (m_State == EnemyState::alive) {
		Mallet* mallet = ItemManager::GetInstance()->GetMallet();
		mallet->Spawn(m_Position);
		Despawn();
	}
}

int Enemy::GetDamage() const {
	if (m_State == EnemyState::alive) {
		return m_Damage;
	}
	return 0;
}

void Enemy::UpdateHitPeriod(float elapsedSec) {
	if (m_IsHit) {

		m_AccuHitPeriod += elapsedSec;
		if (m_AccuHitPeriod >= m_MaxHitPeriod) {
			m_AccuHitPeriod = 0;
			m_IsHit = false;
		}
	}
}