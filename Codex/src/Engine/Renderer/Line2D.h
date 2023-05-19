#ifndef CODEX_RENDERER_LINE_2D_H
#define CODEX_RENDERER_LINE_2D_H

#include "../Core/Geomtryd.h"

namespace Codex {
	class Line2D
	{
	private:
		Vector2f m_Source;
		Vector2f m_Destination;
		Vector4f m_Colour;
		int m_LifeTime;

	public:
		Line2D(Vector2f source, Vector2f destination, Vector4f colour, int lifeTime) :
			m_Source(source), m_Destination(destination), m_Colour(colour), m_LifeTime(lifeTime)
		{

		}

	public:
		inline Vector2f GetSource() const { return m_Source; }
		inline Vector2f GetDestination() const { return m_Destination; }
		inline Vector4f GetColour() const { return m_Colour; }
		inline int GetLifeTime() const { return m_LifeTime; }

	public:
		int BeginFrame()
		{
			return --m_LifeTime;
		}
	};
}

#endif // CODEX_RENDERER_LINE_2D_H