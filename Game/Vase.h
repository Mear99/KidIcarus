#pragma once
#include "Item.h"

class Vase final : public Item
{
	public:
		explicit Vase(float screenwidth, Tilemap* itemMap);
		virtual bool CheckCollision(const Player& player) const override;
		bool CheckCollision(const Bullet& bullet) const;
		virtual void OnPickUp() override;
};

