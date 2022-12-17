#pragma once
#include "Enemy.h"

class Shemum final : public Enemy
{
	public:
		explicit Shemum(float screenWidth, Texture* texture);
		virtual void Update(float elapsedSec, const Level& level) override;

	private:
		const float m_Speed;
		bool IsAtWall(const Level& level);
};

