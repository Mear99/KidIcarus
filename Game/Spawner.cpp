#include "pch.h"
#include "Spawner.h"
#include "Enemy.h"
#include "utils.h"

Spawner::Spawner(const Point2f& screenOffset, int amount, EnemyType type, const Rectf& spawnRegion)
	: GameObject(Point2f{ spawnRegion.left + screenOffset.x, spawnRegion.bottom + screenOffset.y })
	, m_Amount{ amount }
	, m_Type{ type }
	, m_pEnemies{ new Enemy*[amount]{ nullptr } }
	, m_EnemiesAlive{ 0 }
	, m_AccuTime{ 0 }
	, m_SpawnDelay{ 0.2f }
	, m_CoolDownTime{ 5.0f }
	, m_State{ SpawnerState::disabled }
	, m_SpawnRegion{ spawnRegion }
	, m_ScreenOffset{ screenOffset }
	, m_SuccessfullSpawns{ 0 }
{
}

Spawner::~Spawner() {
	delete[] m_pEnemies;
}

void Spawner::Update(float elapsedSec, EnemyManager& manager, const Rectf& camera, const Level& level) {
	
	if (m_SpawnRegion.width != 0 && m_SpawnRegion.height != 0) {
		CheckSpawnArea(camera);
	}

	switch (m_State) {
		case SpawnerState::cooldown:
			m_AccuTime += elapsedSec;
			if (m_AccuTime > m_CoolDownTime) {
				m_State = SpawnerState::spawning;
				m_AccuTime = 0;
			}
			break;
		case SpawnerState::waiting:
			if (m_EnemiesAlive == 0) {
				m_State = SpawnerState::cooldown;
				m_AccuTime = 0;
			}
			break;

		case SpawnerState::spawning:
			m_AccuTime += elapsedSec;
			if (m_AccuTime > m_SpawnDelay) {

				SpawnEnemy(manager, level);

				m_AccuTime -= m_SpawnDelay;
				if (m_SuccessfullSpawns == m_Amount) {
					m_State = SpawnerState::waiting;
					m_SuccessfullSpawns = 0;
				}
			}
			break;
	}
}

void Spawner::SpawnEnemy(EnemyManager& manager, const Level& level) {
	if (!m_pEnemies[m_EnemiesAlive]) {
		m_pEnemies[m_EnemiesAlive] = manager.CreateEnemy(m_Type);
	}
	m_pEnemies[m_EnemiesAlive]->Spawn(m_Position, this);

	// When the enemy spawns inside the level, despawn it try again next frame
	if (level.Collides(*m_pEnemies[m_EnemiesAlive])) {
		m_pEnemies[m_EnemiesAlive]->Despawn();
		--m_SuccessfullSpawns;
	}

	// Add an alive enemy to the counter, 
	// the despawn function decrements the counter, so if spawning fails the counter will still be correct
	++m_EnemiesAlive;
	++m_SuccessfullSpawns;
}

void Spawner::EnemyDespawned() {
	--m_EnemiesAlive;
}

void Spawner::CheckSpawnArea(const Rectf& camera) {
	if (utils::IsOverlapping(camera, m_SpawnRegion) && camera.bottom >= m_SpawnRegion.bottom) {

		m_Position.y = camera.bottom + m_ScreenOffset.y;
		m_State = (m_State == SpawnerState::disabled) ? SpawnerState::spawning : m_State;
	}
	else {
		m_State = (m_State != SpawnerState::disabled) ? SpawnerState::disabled : m_State;
	}
}

void Spawner::DeactivateSpawner() {
	m_State = SpawnerState::disabled;
}

void Spawner::ActivateSpawner() {
	m_State = (m_EnemiesAlive < m_Amount) ? SpawnerState::spawning : SpawnerState::waiting;
}