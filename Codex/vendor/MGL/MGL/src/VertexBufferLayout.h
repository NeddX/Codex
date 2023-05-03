#ifndef MGL_VERTEX_BUFFER_LAYOUT_H
#define MGL_VERTEX_BUFFER_LAYOUT_H

#include <vector>
#include <cstdint>
#include <glad/glad.h>

#include "Constants.h"

namespace mgl
{
	struct VertexBufferElement
	{
	public:
		uint32_t type;
		uint32_t count;
		bool normalized;

	public:
		VertexBufferElement(uint32_t type, uint32_t count, bool normalized)
			: type(type), count(count), normalized(normalized)
		{

		}
	
	public:
		static uint32_t GetSizeOfType(uint32_t type)
		{
			switch (type)
			{
				case GL_UNSIGNED_INT:
				case GL_FLOAT:
				case GL_INT:			return sizeof(GLint);
				case GL_UNSIGNED_BYTE:
				case GL_BYTE:			return sizeof(GLbyte);
				case GL_UNSIGNED_SHORT:
				case GL_SHORT:			return sizeof(GLshort);
				case GL_DOUBLE:			return sizeof(GLdouble);
			}
			ASSERT(false);
			return 0;
		}
	};

	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> m_Elements;
		uint32_t m_Stride;

	public:
		VertexBufferLayout() : m_Stride(0) {};

	public:
		inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }
		inline void Clear()
		{
			m_Elements.clear();
			m_Stride = 0;
		}

	public:
#ifndef __GNUC__
		template<typename T>
		void Push(uint32_t count)
		{
			static_assert(false, "T must inherit from following types:\n\tfloat\n\tint\n\tuint\n\tint8\n\tuint8");
		}
		template<>
		void Push<float>(uint32_t count)
		{
			m_Elements.emplace_back(GL_FLOAT, count, GL_FALSE);
			m_Stride += sizeof(GLfloat) * count;
		}
		template<>
		void Push<uint32_t>(uint32_t count)
		{
			m_Elements.emplace_back(GL_UNSIGNED_INT, count, GL_FALSE);
			m_Stride += sizeof(GLuint) * count;
		}
		template<>
		void Push<uint8_t>(uint32_t count)
		{
			m_Elements.emplace_back(GL_UNSIGNED_BYTE, count, GL_TRUE);
			m_Stride += sizeof(GLubyte) * count;
		}
#else
		template<typename T>
		void Push(uint32_t count)
		{
			m_Elements.emplace_back(GL_FLOAT, count, GL_FALSE);
			m_Stride += sizeof(T) * count;
		}
#endif
	};
}

#endif // MGL_VERTEX_BUFFER_LAYOUT_H