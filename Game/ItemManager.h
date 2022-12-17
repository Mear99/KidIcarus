#pragma once
#include "Item.h"
#include "HeartPickup.h"
#include "Mallet.h"
#include <vector>
#include "Level.h"
#include "Player.h"
class Tilemap;
class Enemy;

class ItemManager final
{
	public:
		static ItemManager* GetInstance(float screenwidth = 0);

		ItemManager(const ItemManager& other) = delete;
		ItemManager(ItemManager&& other) = delete;
		ItemManager& operator=(const ItemManager& other) = delete;
		ItemManager& operator=(ItemManager&& other) = delete;

		~ItemManager() = default;

		void CleanUp();

		void Draw() const;
		void Update(float elapsedSec, const Rectf& camera);
		void CheckCollisions(const Player& player);
		void PlayHarp();
		bool IsHarpPlaying() const;
		HeartPickup* GetHeart();
		Mallet* GetMallet();
		void LoadFromFile(const std::string& filepath);
		std::vector<Item*> GetItems();

	private:
		ItemManager(float screenwidth);

		static ItemManager* m_pManager;

		std::vector<Item*> m_pItems;
		Tilemap* m_pItemMap;
		float m_ScreenWidth;

		bool m_HarpIsPlaying;
		const float m_MaxHarpTime{ 10.0f };
		float m_AccuHarpTime;
		float m_AccuBeepInterval{ 0 };
		const float m_MaxBeepInterval{ 0.5f };
};

