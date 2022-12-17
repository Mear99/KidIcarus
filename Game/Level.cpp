#include "pch.h"
#include "Level.h"
#include "Tile.h"
#include "CollidableTile.h"
#include "DamageTile.h"
#include "Platform.h"
#include "Tilemap.h"
#include "Actor.h"
#include "Player.h"
#include <fstream>
#include <sstream>
#include <typeinfo>
#include "SoundManager.h"

Level::Level() 
	: m_Rows{ 0 }
	, m_Cols { 0 }
	, m_pGrid{ new Tile*  [1] {nullptr} }
	, m_pTilemap{ new Tilemap{"Resources/Tiles/Tilemap.png", 16.0f } }
{
}

Level::~Level() {
	CleanUp();
}

void Level::Update(float elapsedSec) {
	for (int i{ 0 }; i < m_Rows * m_Cols; ++i) {

		DamageTile* damageTile{ dynamic_cast<DamageTile*>(m_pGrid[i]) };

		if (damageTile) {
			damageTile->Update(elapsedSec);
		}
	}
}

int Level::CheckCollisions(const Player& player) {
	for (int i{ 0 }; i < m_Rows * m_Cols; ++i) {

		DamageTile* damageTile{ dynamic_cast<DamageTile*>(m_pGrid[i]) };
		
		if (damageTile && damageTile->CheckCollision(player)) {
			return damageTile->GetDamage();
		}
	}
	return 0;
}

void Level::CleanUp() {
	for (int i{ 0 }; i < m_Rows * m_Cols; ++i) {
		delete m_pGrid[i];
	}
	delete[] m_pGrid;

	delete m_pTilemap;
}

void Level::DrawBackground() const {
	for (int i{ 0 }; i < m_Rows * m_Cols; ++i) {
		if (m_pGrid[i]) {
			if (typeid(*m_pGrid[i]) == typeid(CollidableTile)) {
				m_pGrid[i]->Draw();
			}
		}
	}
}

void Level::DrawForeground() const {
	for (int i{ 0 }; i < m_Rows * m_Cols; ++i) {
		if (m_pGrid[i]) {
			if (typeid(*m_pGrid[i]) != typeid(CollidableTile)) {
				m_pGrid[i]->Draw();
			}
		}
	}
}

bool Level::Collides(const Actor& actor) const {
	for (int i{ 0 }; i < m_Rows * m_Cols; ++i) {
		if (m_pGrid[i]) {
			if (m_pGrid[i]->Collides(actor)) {
				return true;
			}
		}
	}
	return false;
}

float Level::DistanceToRowBottom(const Point2f& pos) const {
	return pos.y - int(pos.y / m_pTilemap->GetTileSize()) * m_pTilemap->GetTileSize();
}

float Level::DistanceToRowTop(const Point2f& pos) const {
	const float result{ m_pTilemap->GetTileSize() - DistanceToRowBottom(pos) };
	return result >= m_pTilemap->GetTileSize() ? 0.0f : result;
}

float Level::DistanceToColLeft(const Point2f& pos) const {
	return pos.x - int(pos.x / m_pTilemap->GetTileSize()) * m_pTilemap->GetTileSize();
}

float Level::DistanceToColRight(const Point2f& pos) const {
	const float result{ m_pTilemap->GetTileSize() - DistanceToColLeft(pos) };
	return result >= m_pTilemap->GetTileSize() ? 0.0f : result;
}

Rectf Level::GetBoundaries() const {
	return Rectf{ 0, 0, m_Cols * m_pTilemap->GetTileSize(), m_Rows * m_pTilemap->GetTileSize() };
}

bool Level::ReachedEnd(const Player& player) {
	return player.CheckCollision(m_EndZone);
}

void Level::LoadFromFile(std::string path) {

	CleanUp();

	std::ifstream inputStream{ path };
	if (inputStream) {

		std::string line{};

		// Until the end of the spawners block
		getline(inputStream, line, '}');
		std::stringstream metadata{ line };
		while (metadata) {
			std::string identifier{};
			char bracket{};
			int rows{}, cols{};
			std::string tilemapPath{}, music{};
			Rectf endZone{};

			metadata >> bracket;
			if (bracket == '{') {
				
				metadata >> identifier >> rows
						 >> identifier >> cols
						 >> identifier >> tilemapPath
						 >> identifier >> endZone.left >> endZone.bottom >> endZone.width >> endZone.height
						 >> identifier >> music;
				
				m_Rows = rows;
				m_Cols = cols;
				m_pGrid = new Tile * [rows * cols]{ nullptr };
				m_pTilemap = new Tilemap{ tilemapPath, 16.0f };
				m_EndZone = endZone;
				SoundManager::GetInstance()->PlayMusic(music);
			}
		}

		char tileID{};

		for (int i{ 0 }; i < m_Rows; ++i) {
			for(int j{ 0 }; j < m_Cols; ++j){

				do {
					inputStream.get(tileID);
				} while (tileID == '\n');

				switch (tileID) {
					// nothing
					case '0':
						break;
					// bricks
					case '1':
						m_pGrid[i * m_Cols + j] = new CollidableTile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 0, 0 };
						break;
					// ground
					case '2':
						m_pGrid[i * m_Cols + j] = new CollidableTile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 0, 1 };
						break;
					// purple door bottom open
					case '3':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 4, 0 };
						break;
					// purple door top open
					case '4':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 3, 0 };
						break;
					// purple door bottom closed
					case '5':
						m_pGrid[i * m_Cols + j] = new CollidableTile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 4, 1 };
						break;
					// purple door top closed
					case '6':
						m_pGrid[i * m_Cols + j] = new CollidableTile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 3, 1 };
						break;
					// bush
					case '7':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 3, 3 };
						break;
					// pillar bottom
					case '8':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 2, 4 };
						break;
					// pillar middle
					case '9':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 1, 4 };
						break;
					// pillar top
					case 'A':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 0, 4 };
						break;
					// pillar broken end
					case 'B':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 4, 4 };
						break;
					// pillar broken middle
					case 'C':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 3, 4 };
						break;
					// yellow door bottom open
					case 'D':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 2, 0 };
						break;
					// yellow door top open
					case 'E':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 1, 0 };
						break;
					// yellow door bottom closed
					case 'F':
						m_pGrid[i * m_Cols + j] = new CollidableTile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 2, 1 };
						break;
					// yellow door top closed
					case 'G':
						m_pGrid[i * m_Cols + j] = new CollidableTile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 1, 1 };
						break;
					// purple plant normal
					case 'H':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 4, 2 };
						break;
					// purple plant end
					case 'I':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 3, 2 };
						break;
					// brown plant normal
					case 'J':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 2, 2 };
						break;
					// brown plant end
					case 'K':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 1, 2 };
						break;
					// platform
					case 'L':
						m_pGrid[i * m_Cols + j] = new Platform{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap };
						break;
					// big brick
					case 'M':
						m_pGrid[i * m_Cols + j] = new CollidableTile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 0, 3 };
						break;
					// purple pillar end
					case 'N':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 2, 3 };
						break;
					// purple pillar normal
					case 'O':
						m_pGrid[i * m_Cols + j] = new Tile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 1, 3 };
						break;
					// blue block
					case 'P':
						m_pGrid[i * m_Cols + j] = new CollidableTile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 4, 3 };
						break;
					// Golden bricks 1
					case 'Q':
						m_pGrid[i * m_Cols + j] = new CollidableTile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 1, 5 };
						break;
					// Golden bricks 2
					case 'R':
						m_pGrid[i * m_Cols + j] = new CollidableTile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 3, 5 };
						break;
					// Golden bricks 3
					case 'S':
						m_pGrid[i * m_Cols + j] = new CollidableTile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 4, 5 };
						break;
					// Golden scales
					case 'T':
						m_pGrid[i * m_Cols + j] = new CollidableTile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap, 2, 5 };
						break;
					// Lava
					case 'U':
						m_pGrid[i * m_Cols + j] = new DamageTile{ Point2f{m_pTilemap->GetTileSize() * j, m_pTilemap->GetTileSize() * i}, m_pTilemap };
						break;
				}
			}
		}
	}
}