#pragma once
#include "Enemy.h"
#include "Player.h"
class Spawner;

enum class ReaperAction { patrol, panic, lookback };

class Reaper final : public Enemy
{
	public:
		explicit Reaper(float screenWidth, Texture* texture);
		virtual void Update(float elapsedSec, const Level& level) override;
		void SetPatrolPoints(const Point2f& left, const Point2f& right);
		void CheckSight(const Player& player);
		virtual void Despawn() override;

	private:
		const float m_Speed;
		const float m_PanicSpeed;
		ReaperAction m_Action;
		Point2f m_PatrolLeft, m_PatrolRight;
		const float m_TrackingDistance;
		float m_AccuPanicTime;
		const float m_MaxPanicTime;
		float m_AccuLookbackTime;
		const float m_MaxLookbackTime;
		const float m_LookbackDuration;
		Spawner* m_pReapetteSpawner;

		void Panic(float elapsedSec);
		void Patrol(float elapsedSec);
		void Lookback(float elapsedSec);
		void StartPanic();
		void StopPanic();
		void CheckLookback(float elapsedSec);
};

