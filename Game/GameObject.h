#pragma once

class GameObject
{
	public:
		virtual ~GameObject() = default;

	protected:
		explicit GameObject(const Point2f& pos);
		Point2f m_Position;
};