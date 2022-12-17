#include "pch.h"
#include "Player.h"
#include "Level.h"
#include "Bullet.h"
#include "BulletManager.h"
#include <iostream>
#include "SoundManager.h"

int Player::m_Health{ 7 };

Player::Player(const Rectf& hitbox, float screenWidth, Texture* texture, int rows, int cols)
	: Actor(hitbox, screenWidth, texture, rows, cols)
	, m_State{ State::falling }
	, m_Direction{ Direction::right }
	, m_WalkSpeed{ 50.0f }
	, m_JumpForce{ 165.0f }
	, m_IsShooting{ false }
	, m_IsInvulnerable{ false }
	, m_GracePeriod{ 2.0f }
	, m_AccuGracePeriod{ 0 }
{
}

void Player::Update(float elapsedSec, const Level& level) {

	if (m_State != State::dead) {
		// Without disturbance these are the applied forces
		m_Velocity.x = 0;
		m_Velocity.y -= IsOnGround(level) ? m_Velocity.y : m_Gravity * elapsedSec;

		// Apply forces by the player
		CheckKeys(level);
		UpdateState(level);
	}

	switch (m_State) {
		case State::idle:
			break;

		case State::jumping:
		case State::falling:
		case State::crouching:
		case State::moving:
		case State::aiming:
			MoveHorizontal(elapsedSec, level);
			MoveVertical(elapsedSec, level);
			break;

		case State::dead:
			m_Velocity.y = m_WalkSpeed;
			Translate(0, -m_Velocity.y*elapsedSec);
	}

	UpdateInvurnerability(elapsedSec);
	UpdateSprite(elapsedSec);
}

void Player::CheckKeys(const Level& level) {

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_A]) {
		m_Direction = Direction::left;
		m_Sprite.SetFlip(true);
		m_Velocity.x -= m_WalkSpeed;
	}
	if (pStates[SDL_SCANCODE_D]) {
		m_Direction = Direction::right;
		m_Sprite.SetFlip(false);
		m_Velocity.x += m_WalkSpeed;
	}
	if (pStates[SDL_SCANCODE_W]) {
		m_Velocity.x = 0;
	}

	if (!pStates[SDL_SCANCODE_S] && m_State == State::crouching) {
		StopCrouch();
	}

	if (pStates[SDL_SCANCODE_S]) {
		if (m_Velocity.y > 0) {
			m_Velocity.y = 0;
		}
		if (m_State != State::crouching) {
			StartCrouch();
		}
	} 
	else if (pStates[SDL_SCANCODE_E]) {
		if (!m_IsShooting) {
			m_IsShooting = true;
			ShootArrow();
		}
	}
	else if (pStates[SDL_SCANCODE_SPACE] && IsOnGround(level)) {
 		m_Velocity.y = m_JumpForce;
		SoundManager::GetInstance()->PlayEffect("Jump");
	}
}

void Player::UpdateState(const Level& level) {

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_S]) {
		m_State = State::crouching;
	}
	else if (pStates[SDL_SCANCODE_W]) {
		m_State = State::aiming;
	}
	else if (m_Velocity.y > 0) {
		m_State = State::jumping;
	}
	else if (!IsOnGround(level)) {
		m_State = State::falling;
	}
	else if (m_Velocity.x != 0) {
		m_State = State::moving;
	}
	else {
		m_State = State::idle;
	}
}

bool Player::IsCrouching() const{
	return m_State == State::crouching;
}

void Player::ShootArrow() {
	Bullet* bullet = BulletManager::GetInstance()->GetBullet(BulletType::arrow);

	Point2f pos{ };
	pos.x = m_Position.x + m_Hitbox.width / 2;
	pos.y = m_Position.y + m_Hitbox.height / 2;
	Vector2f velocity{};

	velocity.x = (m_State == State::aiming) ? 0 : m_WalkSpeed * 2 * int(m_Direction);
	velocity.y = (m_State == State::aiming) ? m_WalkSpeed * 2 : 0;

	bullet->Shoot(pos, velocity, this);
	SoundManager::GetInstance()->PlayEffect("Shoot");
}

void Player::BulletDespawned() {
	m_IsShooting = false;
}

void Player::TakeDamage(int damage) {
	if (!m_IsInvulnerable && damage > 0) {
		m_Health -= damage;
		m_IsInvulnerable = true;
		std::cout << "Health: " << m_Health << "/7\n";

		SoundManager::GetInstance()->PlayEffect("Damage");

		if (m_Health <= 0) {
			m_State = State::dead;
		}
	}
}

bool Player::CheckDead(float DeathPlaneY) {
	if (DeathPlaneY > m_Position.y + m_Hitbox.height) {
		m_State = State::dead;
		std::cout << "You died!\n";
		return true;
	}
	return false;
}

void Player::StartCrouch() {
	m_Hitbox.height /= 2;
	m_State = State::crouching;
}

void Player::StopCrouch() {
	m_Hitbox.height *= 2;
	m_State = State::falling;
}

void Player::UpdateSprite(float elapsedSec) {
	int row{ int(m_State) };
	row += m_IsShooting * (int(State::dead) + 1);
	m_Sprite.SetRow(row);
	m_Sprite.SetVariant(m_IsInvulnerable);
	m_Sprite.Update(elapsedSec);
}

void Player::UpdateInvurnerability(float elapsedSec) {
	if (m_IsInvulnerable) {
		m_AccuGracePeriod += elapsedSec;
		if (m_AccuGracePeriod >= m_GracePeriod) {
			m_IsInvulnerable = false;
			m_AccuGracePeriod = 0;
		}
	}
}

void Player::Heal() {
	m_Health = 7;
	std::cout << "Health: " << m_Health << "/7\n";
}

void Player::Teleport(const Point2f& pos) {
	SetPosition(pos);
}

int Player::GetHealth() const {
	return m_Health;
}