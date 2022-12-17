#pragma once
#include "Item.h"

class Mallet final : public Item
{
	public:
		explicit Mallet(float screenwidth, Tilemap* itemMap);
		virtual void OnPickUp() override;
		void Update(float elapsedSec, const Rectf& boundingBox);

	private:
		const float m_FallSpeed;
};

