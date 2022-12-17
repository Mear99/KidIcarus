#pragma once
#include "Item.h"
#include "Player.h"

enum class HeartType { small, medium, large };

class HeartPickup final : public Item
{
	public:
		explicit HeartPickup(HeartType type, float screenwidth, Tilemap* itemMap);
		void SetSize(HeartType type);
		virtual void OnPickUp() override;
		static int GetScore();

	private:
		HeartType m_Type;
		static int m_Hearts;
};

