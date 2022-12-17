#include "pch.h"
#include "Camera.h"
#include "utils.h"

Camera::Camera(float width, float height, float scale)
	: m_Width{ width/scale }
	, m_Height{ height/scale }
	, m_Scale{ scale }
	, m_Speed{ 0 }
	, m_MaxSpeed{ 50.0f }
	, m_DeltaSpeed{ 10.0f }
{
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries) {
	m_LevelBoundaries = levelBoundaries;
	m_BottomLeft.x = levelBoundaries.left;
	m_BottomLeft.y = levelBoundaries.bottom;
}

void Camera::Update(const Rectf& target, float elapsedSec) {
	Track(target, elapsedSec);
	Clamp();
}

void Camera::Transform() const {
	glScalef(m_Scale, m_Scale, 1);
	glTranslatef(-m_BottomLeft.x, -m_BottomLeft.y, 0);
}

void Camera::Track(const Rectf& target, float elapsedSec) {
	m_BottomLeft.x = (target.left + target.width / 2) - (m_Width / 2);
	const float newY{ (target.bottom + target.height / 2) - (m_Height / 2) };

	// Camera ease in
	if(newY > m_BottomLeft.y) {

		m_Speed += m_DeltaSpeed;
		m_Speed = (m_Speed > m_MaxSpeed) ? m_MaxSpeed : m_Speed;
		m_BottomLeft.y += m_Speed * elapsedSec;
	}
	// And ease out
	else {
		m_Speed -= m_DeltaSpeed;
		m_Speed = (m_Speed < 0) ? 0 : m_Speed;

		m_BottomLeft.y += m_Speed * elapsedSec;
	}
}

void Camera::Clamp() {

	m_BottomLeft.x = (m_BottomLeft.x < m_LevelBoundaries.left) ? m_LevelBoundaries.left : m_BottomLeft.x;
	m_BottomLeft.x = (m_BottomLeft.x > m_LevelBoundaries.left + m_LevelBoundaries.width - m_Width) ? m_LevelBoundaries.left + m_LevelBoundaries.width - m_Width : m_BottomLeft.x;
	m_BottomLeft.y = (m_BottomLeft.y < m_LevelBoundaries.bottom) ? m_LevelBoundaries.bottom : m_BottomLeft.y;
	m_BottomLeft.y = (m_BottomLeft.y > m_LevelBoundaries.bottom + m_LevelBoundaries.height - m_Height) ? m_LevelBoundaries.bottom + m_LevelBoundaries.height - m_Height : m_BottomLeft.y;
}

Rectf Camera::GetShape() const {
	return Rectf{ m_BottomLeft.x, m_BottomLeft.y, m_Width, m_Height };
}

void Camera::SnapTo(const Rectf& target) {
	m_BottomLeft.x = (target.left + target.width / 2) - (m_Width / 2);
	m_BottomLeft.y = (target.bottom + target.height / 2) - (m_Height / 2);

	Clamp();
}
