#pragma once
#include "Enemy.h"

enum class MonoeyeAction{ hovering, lockDive, diving };

class Monoeye final : public Enemy
{
	public:
		explicit Monoeye(float screenWidth, Texture* texture);
		void SetTrackingPoint(const Point2f& point);
		virtual void Update(float elapsedSec, const Level& level) override;
		virtual bool CheckCollision(const Rectf& hitbox) const override;

	private:
		const float m_Speed{ 150.0f };
		const float m_HoverSpeed{ 50.0f };
		float m_AccuAngle;
		MonoeyeAction m_Action;
		Point2f m_TrackingPoint;
		const float m_TrackingDistance{ 100.0f };
		float m_AccuHoverTime;
		const float m_MaxHoverTime;

		void Hover(float elapsedSec);
		void Dive(float elapsedSec);

};

