#pragma once
class Sprite;
class Actor;
class Player;
class Texture;
class ItemManager;
class BulletManager;
class SpawnerManager;
class SoundManager;
#include "Level.h"
#include "Camera.h"
#include "EnemyManager.h"
#include <vector>
#include "DoorManager.h"
#include "HUD.h"

enum class GameState { menu, playing, dead, win };

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;

	Player* m_pPlayer;
	Texture* m_pPlayerTexture;

	BulletManager* m_pBulletManager;
	SoundManager* m_pSoundManager;
	SpawnerManager* m_pSpawnerManager;
	ItemManager* m_pItemManager;
	DoorManager m_DoorManager;
	EnemyManager m_EnemyManager;

	Level m_Level;
	Camera m_Camera;
	HUD m_HUD;
	GameState m_State;
	Tilemap* m_pScreens;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	void UpdateScreen();
	void DrawScreen(int screen) const;

	void UpdatePositions(float elapsedSec);
	void CheckCollisions();
	void PrintInstructions();

	void LoadLevel(std::string levelName);
};