#pragma once
#include "Enemy.h"

enum class McGooAction { pickLocation, uncover, shoot, shooting, idle };

class McGoo final : public Enemy
{
public:
	explicit McGoo(float screenWidth, Texture* texture);
	virtual void Update(float elapsedSec, const Level& level) override;
	void SetTrackingPoint(const Point2f& point);
	virtual bool CheckHit(const Player& player) const override;
	void BulletDespawned();
	void ShootBullet();

private:
	Point2f m_TrackingPoint;
	McGooAction m_Action;
	float m_AccuUncoverTime;
	const float m_MaxUncoverTime;
	const float m_BulletSpeed;
};

