#pragma once
#include "Item.h"
class Harp final : public Item
{
	public: 
		explicit Harp(float screenwidth, Tilemap* itemMap);
		virtual void OnPickUp() override;
};

