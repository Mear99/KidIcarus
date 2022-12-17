#pragma once

class Camera final
{
public:
	explicit Camera(float width, float height, float scale);

	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Update(const Rectf& target, float elapsedSec);
	void Transform() const;
	Rectf GetShape() const;
	void SnapTo(const Rectf& target);

private:
	float m_Width;
	float m_Height;
	Rectf m_LevelBoundaries;
	Point2f m_BottomLeft;
	float m_Scale;
	float m_Speed;
	const float m_MaxSpeed;
	const float m_DeltaSpeed;

	void Track(const Rectf& target, float elapsedSec);
	void Clamp();

};

