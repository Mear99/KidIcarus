#include "pch.h"
#include "BulletManager.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Vase.h"
#include "Texture.h"

BulletManager* BulletManager::m_pManager{ nullptr };

BulletManager* BulletManager::GetInstance(float screenwidth) {
	if (!m_pManager) {
		m_pManager = new BulletManager(screenwidth);
	}

	return m_pManager;
}

BulletManager::BulletManager(float screenwidth) 
	: m_ScreenWidth{ screenwidth }
{
	m_pBulletTextures.push_back(new Texture{ "Resources/Sprites/arrow.png" });
	m_pBulletTextures.push_back(new Texture{ "Resources/Sprites/bullet.png" });
}

void BulletManager::CleanUp() {

	for (Bullet* bullet : m_pBullets) {
		delete bullet;
	}
	for (Texture* texture : m_pBulletTextures) {
		delete texture;
	}

	delete m_pManager;

}

void BulletManager::Update(float elapsedSec, const Level& level) {
	for (Bullet* bullet : m_pBullets) {
		bullet->Update(elapsedSec, level);
	}
}

void BulletManager::Draw() const {
	for (Bullet* bullet : m_pBullets) {
		bullet->Draw();
	}
}

Bullet* BulletManager::GetBullet(BulletType type) {
	for (Bullet* bullet : m_pBullets) {
		if (!bullet->IsActive() && bullet->GetType() == type) {
			return bullet;
		}
	}
	m_pBullets.push_back(new Bullet{ Rectf{0,0,8,8}, m_ScreenWidth, m_pBulletTextures[int(type)], 2,1, type});
	return m_pBullets[m_pBullets.size() - 1];
}

bool BulletManager::CheckCollisions(const Level& level) {
	for (Bullet* bullet : m_pBullets) {
		if (level.Collides(*bullet)) {
			bullet->Despawn();
			return true;
		}
	}
	return false;
}

bool BulletManager::CheckCollisions(std::vector<Enemy*> enemies) {
	for (Bullet* bullet : m_pBullets) {
		if (bullet->GetType () == BulletType::arrow) {
			for (Enemy* enemy : enemies) {
				if (enemy->CheckHit(*bullet)) {
					enemy->TakeDamage(bullet->GetDamage());
					bullet->Despawn();
					return true;
				}
			}
		}
	}
	return false;
}
bool BulletManager::CheckCollisions(std::vector<Item*> items) {
	for (Bullet* bullet : m_pBullets) {
		if (bullet->GetType() == BulletType::arrow) {
			for (Item* item : items) {
				Vase* vase{ dynamic_cast<Vase*>(item) };
				if (vase) {
					if (vase->CheckCollision(*bullet)) {
						vase->OnPickUp();
						vase->Despawn();

						bullet->Despawn();
						return true;
					}
				}
			}
		}
	}
	return false;
}


int BulletManager::CheckCollisions(const Player& player) {
	for (Bullet* bullet : m_pBullets) {
		if (bullet->GetType() == BulletType::bullet && bullet->CheckCollision(player.GetHitbox())) {
			return bullet->GetDamage();
		}	
	}
	return 0;
}