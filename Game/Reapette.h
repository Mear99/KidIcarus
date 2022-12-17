#pragma once
#include "Enemy.h"

enum class ReapetteAction { hovering, lockDive, diving };

class Reapette final : public Enemy
{
public:
	explicit Reapette(float screenWidth, Texture* texture);
	void SetTrackingPoint(const Point2f& point);
	virtual void Update(float elapsedSec, const Level& level) override;
	virtual bool CheckCollision(const Rectf& hitbox) const override;

private:
	const float m_Speed;
	float m_AccuAngle;
	ReapetteAction m_Action;
	Point2f m_TrackingPoint;
	float m_AccuHoverTime;
	const float m_MaxHoverTime;

	void Hover(float elapsedSec);
	void Dive(float elapsedSec);
	void CheckDive();

};
