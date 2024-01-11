#ifndef CODEX_RENDERER_LINE_2D_H
#define CODEX_RENDERER_LINE_2D_H

#include "../Core/Geomtryd.h"

namespace codex::renderer {
    class Line2D
    {
    private:
        Vector2f m_Source;
        Vector2f m_Destination;
        Vector4f m_Colour;
        i32      m_LifeTime;

    public:
        Line2D(const Vector2f source, const Vector2f destination, const Vector4f colour, const i32 lifeTime)
            : m_Source(source), m_Destination(destination), m_Colour(colour), m_LifeTime(lifeTime)
        {
        }

    public:
        inline Vector2f GetSource() const { return m_Source; }
        inline Vector2f GetDestination() const { return m_Destination; }
        inline Vector4f GetColour() const { return m_Colour; }
        inline i32      GetLifeTime() const { return m_LifeTime; }

    public:
        i32 BeginFrame() { return --m_LifeTime; }
    };
} // namespace codex::renderer

#endif // CODEX_RENDERER_LINE_2D_H
