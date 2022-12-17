#pragma once
#include "Actor.h"
#include "Vector2f.h"

enum class BulletType { arrow, bullet };

class Bullet final : public Actor
{
	public:
		explicit Bullet(const Rectf& hitbox, float screenWidth, Texture* texture, int rows, int cols, BulletType type);
		virtual void Draw() const override ;
		virtual void Update(float elapsedSec, const Level& level) override;
		void Shoot(const Point2f& pos, const Vector2f& velocity, Actor* owner);
		void Despawn();
		bool Collides(const Actor& actor) const;
		bool IsActive();
		int GetDamage() const;
		BulletType GetType() const;

	private:
		bool m_IsActive;
		Actor* m_pOwner;
		float m_AccuTime;
		const float m_Lifespan{ 0.5f };
		const int m_Damage;
		BulletType m_Type;
};