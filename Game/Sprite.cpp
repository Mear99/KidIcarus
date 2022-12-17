#include "pch.h"
#include "Sprite.h"
#include "Texture.h"

Sprite::Sprite(Texture* texture, int rows, int cols)
	: m_pTexture{ texture }
	, m_Rows{ rows }
	, m_Colums{ 2*cols }
	, m_CurrentFrame{ 0 }
	, m_CurrentRow{ 0 }
	, m_AccumulatedTime { 0.0f }
	, m_FramesPerSec{ 2*cols }
	, m_Flip{ false }
	, m_Loop{ true }
	, m_Variant{ false }
{
}

void Sprite::SetRow(int row) {
	m_CurrentRow = row;
}

void Sprite::Draw() const {

	int column{ (m_Variant ? m_CurrentFrame + (m_Colums / 2) : m_CurrentFrame) };

	Rectf srcRect{};
	srcRect.width = m_pTexture->GetWidth() / m_Colums;
	srcRect.height = m_pTexture->GetHeight() / m_Rows;
	srcRect.left = srcRect.width * column;
	srcRect.bottom = (1 + m_CurrentRow) * srcRect.height;

	glPushMatrix();

	if (m_Flip) {
		glScalef(-1, 1, 1);	
		glTranslatef(-srcRect.width, 0, 0);
	}
		
	m_pTexture->Draw(Point2f{ 0,0 }, srcRect);

	glPopMatrix();

}

void Sprite::Update(float elapsedSec) {
	m_AccumulatedTime += elapsedSec;
	if (m_AccumulatedTime >= 1.0f / m_FramesPerSec) {
		m_AccumulatedTime -= 1.0f / m_FramesPerSec;

		if (m_Loop || m_CurrentFrame < (m_Colums / 2) - 1) {
			++m_CurrentFrame %= (m_Colums / 2);
		}
	}
}

void Sprite::SetFlip(bool flip) {
	m_Flip = flip;
}
void Sprite::ToggleFlip() {
	m_Flip = !m_Flip;
}

void Sprite::SetLoop(bool loop) {
	m_Loop = loop;
	m_CurrentFrame = 0;
}

void Sprite::SetVariant(bool variant) {
	m_Variant = variant;
}