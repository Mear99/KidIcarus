#pragma once
#include "Actor.h"
#include "Level.h"
#include "Bullet.h"
#include "Player.h"
#include "HeartPickup.h"
class Spawner;

enum class EnemyState { alive = 0, dead = 1, idle };

class Enemy : public Actor
{
	public:
		explicit Enemy(const Rectf& hitbox, float screenWidth, Texture* texture, int rows, int cols, int health = 1, int damage = 1);
		virtual ~Enemy() = default;

		virtual void Draw() const override;
		virtual void Update(float elapsedSec, const Level& level) override;
		void Spawn(const Point2f& pos, Spawner* owner);
		void TakeDamage(int damage);
		virtual void Despawn();
		void Transmutate();
		void CheckBounds(const Rectf& bounds);
		int GetDamage() const;
		bool CheckHit(const Bullet& bullet) const;
		virtual bool CheckHit(const Player& player) const;
		bool IsActive();
		void UpdateHitPeriod(float elapsedSec);

	protected:
		EnemyState m_State;
		int m_Health;
		int m_Damage;
		HeartType m_HeartDrop;

		float m_AccuHitPeriod;
		const float m_MaxHitPeriod;
		bool m_IsHit;

	private:
		Spawner* m_Owner;

};

