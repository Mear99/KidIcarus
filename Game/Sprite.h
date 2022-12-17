#pragma once
class Texture;

class Sprite final
{
	public:
		explicit Sprite(Texture*, int rows, int cols);

		void SetRow(int row);
		void Draw() const;
		void Update(float elapsedSec);
		void SetFlip(bool flip);
		void ToggleFlip();
		void SetLoop(bool loop);
		void SetVariant(bool variant);

	private:
		Texture* m_pTexture; // not owner
		int m_Rows;
		int m_Colums;

		int m_CurrentFrame;
		int m_CurrentRow;
		float m_AccumulatedTime;
		int m_FramesPerSec;

		bool m_Flip;
		bool m_Loop;
		bool m_Variant;
};

