#ifndef CODEX_CORE_CAMERA_H
#define CODEX_CORE_CAMERA_H

#include <sdafx.h>

#include "Geomtryd.h"

namespace Codex {
	class Camera
	{

	private:
		int m_Width;
		int m_Height;
		Matrix4f m_Projection;
		Matrix4f m_View;

	public:
		Vector2f position;

	public:
		Camera(int width, int height, Vector2f position = Vector2f(0.0f, 0.0f)) :
			m_Projection(Matrix4f(0.0f)), position(position), m_Width(width), m_Height(height)
		{
			UpdateProjectionMatrix();
		}

	public:
		inline void UpdateProjectionMatrix()
		{
			//m_Width = 32.0f * 40.0f;
			//m_Height = 32 * 21.0f;
			m_Projection = glm::identity<Matrix4f>();
			m_Projection = glm::ortho(0.0f, (float)(m_Width), (float)(m_Height), 0.0f, 0.0f, 100.0f);
			//m_Projection = glm::ortho(0.0f, 32.0f * 40.0f, 0.0f, 32.0f * 21.0f, 0.0f, 100.0f); // what the fuck?
		}
		inline Matrix4f GetViewMatrix()
		{
			Vector3f camera_front(0.0f, 0.0f, -1.0f);
			Vector3f camera_up(0.0f, 1.0f, 0.0f);

			m_View = glm::identity<Matrix4f>();
			m_View = glm::lookAt(
				Vector3f(position.x, position.y, 20.0f),
				camera_front + Vector3f(position.x, position.y, 0.0f),
				camera_up);
			return m_View;
		}
		inline Matrix4f GetProjectionMatrix()	{ return m_Projection; }
		inline int GetWidth() const				{ return m_Width; }
		inline int GetHeight() const			{ return m_Height; }
		inline void SetWidth(int newWidth)		{ m_Width = newWidth; UpdateProjectionMatrix(); }
		inline void SetHeight(int newHeight)	{ m_Height = newHeight; UpdateProjectionMatrix(); }
	};
}

#endif // CODEX_CORE_CAMERA_H