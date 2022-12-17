#pragma once
#include "Item.h"
class Chalice final : public Item
{
public:
	explicit Chalice(float screenwidth, Tilemap* itemMap);
	virtual void OnPickUp() override;
};

