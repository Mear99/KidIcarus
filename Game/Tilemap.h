#pragma once
class Texture;

class Tilemap final
{
	public:
		explicit Tilemap(std::string path, float tileSize);

		Tilemap(const Tilemap& other) = delete;
		Tilemap(Tilemap&& other) = delete;
		Tilemap& operator=(const Tilemap& other) = delete;
		Tilemap& operator=(Tilemap&& other) = delete;

		~Tilemap();

		void DrawTile(int row, int col) const;
		float GetTileSize() const;

	private:
		Texture* m_pTilemap;
		int m_Rows;
		int m_Cols;

		float m_TileSize;

};

