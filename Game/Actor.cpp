#include "pch.h"
#include "Actor.h"
#include "utils.h"

Actor::Actor(const Rectf& hitbox, float screenWidth, Texture* texture, int rows, int cols)
	: GameObject( Point2f{ hitbox.left, hitbox.bottom })
	, m_Hitbox{ hitbox }
	, m_Sprite{ Sprite{ texture, rows, cols } }
	, m_ScreenWidth{ screenWidth }
	, m_Velocity{0,0}
	, m_Gravity{ 9.81f * 24.0f }
{
}

void Actor::Draw() const {
	glPushMatrix();

	glTranslatef(m_Position.x,m_Position.y, 0);
	m_Sprite.Draw();

	if (m_Hitbox.left + m_Hitbox.width > m_ScreenWidth) {

		glTranslatef(-m_ScreenWidth, 0, 0);
		m_Sprite.Draw();
	}

	glPopMatrix();
}

void Actor::Update(float elapsedSec, const Level& level) {

	m_Velocity.y -= IsOnGround(level) ? m_Velocity.y : m_Gravity * elapsedSec;
	MoveHorizontal(elapsedSec, level);
	MoveVertical(elapsedSec, level);

	m_Sprite.Update(elapsedSec);
}

void Actor::Translate(float deltaX, float deltaY) {
	m_Hitbox.left += deltaX;
	m_Hitbox.bottom += deltaY;

	m_Position.x = m_Hitbox.left;
	m_Position.y = m_Hitbox.bottom;
}

void Actor::SetPosition(Point2f pos) {
	m_Position = pos;
	m_Hitbox.left = pos.x;
	m_Hitbox.bottom = pos.y;
}

void Actor::WrapX() {
	if (m_Position.x < 0) {
		Translate(m_ScreenWidth, 0);
	}
	if (m_Position.x > m_ScreenWidth) {
		Translate(-m_ScreenWidth, 0);
	}
}

bool Actor::CheckCollision(const Rectf& hitbox) const{
	const Rectf wrappedHitbox = Rectf{ m_Hitbox.left - m_ScreenWidth, m_Hitbox.bottom, m_Hitbox.width, m_Hitbox.height };

	return (utils::IsOverlapping(m_Hitbox, hitbox) || utils::IsOverlapping(wrappedHitbox, hitbox));
}

Rectf Actor::GetHitbox() const {
	return m_Hitbox;
}

Vector2f Actor::GetVelocity() const{
	return m_Velocity;
}

bool Actor::IsOnGround(const Level& level) {
	Translate(0, -1);
	const bool isOnGround{ level.Collides(*this) };
	Translate(0, 1);
	return isOnGround;
}

void Actor::MoveHorizontal(float elapsedSec, const Level& level) {
	const float delta{ m_Velocity.x * elapsedSec };
	if (delta != 0) {
		Translate(delta, 0);
		if (level.Collides(*this)) {
			if (m_Velocity.x < 0) {
				Translate(level.DistanceToColRight(m_Position), 0);
			}
			else {
				const Point2f rightPos{ m_Position.x + m_Hitbox.width, m_Position.y };
				Translate(-level.DistanceToColLeft(rightPos), 0);
			}
		}
		WrapX();
	}
}

void Actor::MoveVertical(float elapsedSec, const Level& level) {
	const float delta{ m_Velocity.y * elapsedSec };
	if (delta != 0) {
		Translate(0, delta);
		if (level.Collides(*this)) {

			if (m_Velocity.y <= 0) {
				Translate(0, level.DistanceToRowTop(m_Position));
			}
			else {
				const Point2f headPos{ m_Position.x, m_Position.y + m_Hitbox.height };
				Translate(0, -level.DistanceToRowBottom(headPos));
			}

			// Lose momentum
			m_Velocity.y = 0;
		}
	}
}
