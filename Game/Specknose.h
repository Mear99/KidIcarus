#pragma once
#include "Enemy.h"

class Specknose final : public Enemy
{
public:
	explicit Specknose(float screenWidth, Texture* texture);
	virtual void Update(float elapsedSec, const Level& level) override;
	virtual bool CheckCollision(const Rectf& hitbox) const override;
	void SetPatrolPoints(const Point2f& left, const Point2f& right);

private:
	const float m_Speed;
	Point2f m_PatrolLeft, m_PatrolRight;

};

