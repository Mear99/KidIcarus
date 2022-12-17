#pragma once
#include "Actor.h"
#include "Vector2f.h"
#include "Bullet.h"
#include "Level.h"

enum class State { idle, moving, aiming, falling, jumping, crouching, dead };
enum class Direction{ left = -1, right = 1 };

class Player final : public Actor
{
	public:
		explicit Player(const Rectf& hitbox, float screenWidth, Texture* texture, int rows, int cols);
		virtual void Update(float elapsedSec, const Level& level) override;
		bool IsCrouching() const;
		void BulletDespawned();
		void TakeDamage(int damage);
		bool CheckDead(float DeathPlaneY);
		static void Heal();
		void Teleport(const Point2f& pos);
		int GetHealth() const;

	private:
		State m_State;
		Direction m_Direction;
		const float m_WalkSpeed;
		const float m_JumpForce;
		bool m_IsShooting;
		static int m_Health;
		bool m_IsInvulnerable;
		float m_GracePeriod;
		float m_AccuGracePeriod;

		void CheckKeys(const Level& level);
		void UpdateState(const Level& level);
		void UpdateSprite(float elapsedSec);
		void UpdateInvurnerability(float elapsedSec);

		void ShootArrow();
		void StartCrouch();
		void StopCrouch();
};

