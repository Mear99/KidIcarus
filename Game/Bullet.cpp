#include "pch.h"
#include "Bullet.h"
#include "utils.h"
#include "Player.h"
#include "McGoo.h"

Bullet::Bullet(const Rectf& hitbox, float screenWidth, Texture* texture, int rows, int cols, BulletType type)
	: Actor(hitbox, screenWidth, texture, rows, cols)
	, m_IsActive{ false }
	, m_pOwner{ nullptr }
	, m_AccuTime{ 0.0f }
	, m_Damage{ 1 + int(type) }
	, m_Type{ type }
{
}

void Bullet::Update(float elapsedSec, const Level& level) {
	if (m_IsActive) {
		MoveHorizontal(elapsedSec, level);
		MoveVertical(elapsedSec, level);
		WrapX();

		m_AccuTime += elapsedSec;
		if (m_AccuTime >= m_Lifespan) {
			Despawn();
		}

	}
}

void Bullet::Draw() const {
	if (m_IsActive) {
		Actor::Draw();
	}
}

void Bullet::Shoot(const Point2f& pos, const Vector2f& velocity, Actor* owner) {

	m_Velocity = velocity;
	m_IsActive = true;
	m_pOwner = owner;

	if (m_Velocity.x > 0) {
		m_Sprite.SetFlip(false);
	}
	else {
		m_Sprite.SetFlip(true);
	}

	if (m_Velocity.y != 0) {
		m_Sprite.SetRow(1);
	}
	else {
		m_Sprite.SetRow(0);
	}

	SetPosition(pos);
	Translate(-m_Hitbox.width / 2, -m_Hitbox.height / 2);
}

bool Bullet::Collides(const Actor& actor) const {
	if (m_IsActive) {
		return CheckCollision(actor.GetHitbox());
	}
	return false;
}

void Bullet::Despawn() {
	if (m_IsActive) {
		m_IsActive = false;
		m_AccuTime = 0;

		Player* player{ dynamic_cast<Player*>(m_pOwner) };
		if (player) {
			player->BulletDespawned();
		}

		McGoo* mcGoo{ dynamic_cast<McGoo*>(m_pOwner) };
		if (mcGoo) {
			mcGoo->BulletDespawned();
		}
	}
}

bool Bullet::IsActive() {
	return m_IsActive;
}

int Bullet::GetDamage() const {
	if (m_IsActive) {
		return m_Damage;
	}
	return 0;
}

BulletType Bullet::GetType() const {
	return m_Type;
}