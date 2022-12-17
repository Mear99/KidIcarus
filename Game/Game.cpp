#include "pch.h"
#include "Game.h"
#include "Sprite.h"
#include "Actor.h"
#include "Player.h"
#include "Texture.h"
#include "CollidableTile.h"
#include <iostream>
#include "ItemManager.h"
#include "BulletManager.h"
#include "SpawnerManager.h"
#include "SoundManager.h"
#include "DoorTrigger.h"
#include "Tilemap.h"

Game::Game( const Window& window ) 
	:m_Window{ window }
	, m_Level{}
	, m_Camera{ window.width, window.height, 3.0f }
	, m_EnemyManager{ window.width/3.0f }
	, m_State{ GameState::menu }
	, m_DoorManager{ }
	, m_HUD{}
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_pItemManager = ItemManager::GetInstance(m_Window.width / 3.0f);
	m_pBulletManager = BulletManager::GetInstance(m_Window.width / 3.0f);
	m_pSpawnerManager = SpawnerManager::GetInstance();
	m_pSoundManager = SoundManager::GetInstance();

	m_pScreens = new Tilemap{ "Resources/TIles/Screens.png", 256 };

	m_pPlayerTexture = new Texture{ "Resources/Sprites/testSprite.png" };
	m_pPlayer = new Player{ Rectf{48.0f,2080.0f, 16.0f, 24.0f}, m_Window.width / 3.0f, m_pPlayerTexture, 14, 6 }; // 48,80 original pos

	PrintInstructions();
}

void Game::Cleanup( )
{

	m_pItemManager->CleanUp();
	m_pBulletManager->CleanUp();
	m_pSpawnerManager->CleanUp();
	m_pSoundManager->CleanUp();

	delete m_pPlayerTexture;
	delete m_pPlayer;
	delete m_pScreens;
}

void Game::Update( float elapsedSec )
{
	switch (m_State) {
		case GameState::menu:
		case GameState::dead:
		case GameState::win:
			UpdateScreen();
			break;

		case GameState::playing:
			UpdatePositions(elapsedSec);
			CheckCollisions();
			m_HUD.Update(m_pPlayer->GetHealth(), HeartPickup::GetScore());
			if (m_Level.ReachedEnd(*m_pPlayer)) {
				m_State = GameState::win;
			}
			break;
	}
}

void Game::Draw( ) const
{
	ClearBackground( );

	switch (m_State) {

		case GameState::menu:
			DrawScreen(0);
			break;

		case GameState::playing:
			glPushMatrix();

			m_Camera.Transform();

			m_Level.DrawBackground();
			m_pPlayer->Draw();
			m_EnemyManager.Draw();
			m_pBulletManager->Draw();
			m_pItemManager->Draw();
			m_Level.DrawForeground();

			glPopMatrix();

			m_HUD.Draw();
			break;

		case GameState::dead:
			DrawScreen(1);
			break;

		case GameState::win:
			DrawScreen(2);
			break;
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	if (e.keysym.sym == SDLK_i) {
		PrintInstructions();
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::UpdatePositions(float elapsedSec) {
	m_pPlayer->Update(elapsedSec, m_Level);
	m_pSpawnerManager->Update(elapsedSec, m_EnemyManager, m_Camera.GetShape(), m_Level);
	m_EnemyManager.Update(elapsedSec, m_Level, m_Camera.GetShape(), *m_pPlayer);
	m_pItemManager->Update(elapsedSec, m_Camera.GetShape());
	m_pBulletManager->Update(elapsedSec, m_Level);
	m_Level.Update(elapsedSec);
	m_Camera.Update(m_pPlayer->GetHitbox(), elapsedSec);
}

void Game::CheckCollisions() {
	// Bullets with level
	m_pBulletManager->CheckCollisions(m_Level);
	// Bullets with enemies
	m_pBulletManager->CheckCollisions(m_EnemyManager.GetEnemies());
	// Bullets with items
	m_pBulletManager->CheckCollisions(m_pItemManager->GetItems());
	// Items with player
	m_pItemManager->CheckCollisions(*m_pPlayer);
	// Enemy with player
	m_pPlayer->TakeDamage(m_EnemyManager.CheckHit(*m_pPlayer));
	// Bullets with player
	m_pPlayer->TakeDamage(m_pBulletManager->CheckCollisions(*m_pPlayer));
	// Level with player
	m_pPlayer->TakeDamage(m_Level.CheckCollisions(*m_pPlayer));

	// Check door triggers with player
	DoorTrigger* door{ m_DoorManager.CheckDoors(*m_pPlayer) };
	if (door) {
		std::string levelName{};
		Point2f pos;
		door->GetLevel(pos, levelName);
		LoadLevel(levelName);
		m_pPlayer->Teleport(pos);
		m_Camera.SnapTo(m_pPlayer->GetHitbox());
	}

	// Deathplane with player
	if (m_pPlayer->CheckDead(m_Camera.GetShape().bottom)) {
		m_State = GameState::dead;
	}
}

void Game::PrintInstructions() {
	std::cout << "How to Play: (On a QWERTY)\n";
	std::cout << "Q Key: Aim upwards\n";
	std::cout << "A Key: Move Left\n";
	std::cout << "D Key: Move Right\n";
	std::cout << "S Key: Crouch\n";
	std::cout << "E Key: Shoot arrow\n";
	std::cout << "Spacebar: Jump\n";
	std::cout << "\nGet to the top!\n";
	std::cout << "If you fall of the bottom of the screen, you die!\n";
	std::cout << "Left and right edge are connected\n";
}

void Game::LoadLevel(std::string levelname) {
	std::string tilepath{ "Resources/Levels/" + levelname + ".txt" };
	std::string levelDataPath{ "Resources/Levels/" + levelname + "_LevelData.txt" };

	m_Level.LoadFromFile(tilepath);
	m_pSpawnerManager->LoadFromFile(levelDataPath);
	m_EnemyManager.LoadFromFile(levelDataPath);
	m_pItemManager->LoadFromFile(levelDataPath);
	m_DoorManager.LoadFromFile(levelDataPath);

	m_Camera.SetLevelBoundaries(m_Level.GetBoundaries());

	delete m_pPlayer;
	m_pPlayer = new Player{ Rectf{48.0f,80.0f, 16.0f, 24.0f}, m_Window.width / 3.0f, m_pPlayerTexture, 14, 6 };
}

void Game::UpdateScreen() {

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_E]) {
		LoadLevel("1-1");
		m_State = GameState::playing;
		m_pPlayer->Heal();
	}

	switch (m_State) {
	case GameState::menu:
		SoundManager::GetInstance()->PlayMusic("Menu");
		break;
	case GameState::dead:
		SoundManager::GetInstance()->PlayMusic("GameOver", false);
		break;
	case GameState::win:
		SoundManager::GetInstance()->PlayMusic("Win", false);
		break;
	}
}

void Game::DrawScreen(int screen) const {
	glPushMatrix();
	
	glScalef(3, 3, 0);
	m_pScreens->DrawTile(0, screen);

	glPopMatrix();
}
