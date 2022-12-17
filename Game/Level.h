#pragma once
class Tile;
class Actor;
class Player;
class Tilemap;

class Level final
{
	public:
		explicit Level();

		Level(const Level& other) = delete;
		Level(Level&& other) = delete;
		Level& operator=(const Level& other) = delete;
		Level& operator=(Level&& other) = delete;

		~Level();

		void Update(float elapsedSec);
		int CheckCollisions(const Player& player);

		void DrawBackground() const;
		void DrawForeground() const;
		bool Collides(const Actor& actor) const;
		Rectf GetBoundaries() const;
		void LoadFromFile(std::string path);
		void CleanUp();
		bool ReachedEnd(const Player& player);

		// Distance is always positive
		float DistanceToRowBottom(const Point2f& pos) const;
		float DistanceToRowTop(const Point2f& pos) const;
		float DistanceToColLeft(const Point2f& pos) const;
		float DistanceToColRight(const Point2f& pos) const;

	private:
		Tile** m_pGrid;
		int m_Rows;
		int m_Cols;
		Tilemap* m_pTilemap;
		Rectf m_EndZone;

};

