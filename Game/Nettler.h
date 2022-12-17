#pragma once
#include "Enemy.h"

class Nettler final : public Enemy
{
public:
	explicit Nettler(float screenWidth, Texture* texture);
	virtual void Update(float elapsedSec, const Level& level) override;
	void SetTrackingPoint(const Point2f& point);

private:
	const float m_Speed;
	bool IsAtWall(const Level& level);
	Point2f m_TrackingPoint;
};

