#ifndef MGL_CONSTANTS_H
#define MGL_CONSTANTS_H

#include <stdio.h>
#include <glad/glad.h>

#ifdef __GNUC__
#define ASSERT(x) if (!(x)) __builtin_trap()
#elif _MSC_VER
#define ASSERT(x) if (!(x)) __debugbreak()
#elif __clang__
#define ASSERT(x) if (!(x)) __builtin_debugtrap()
#endif

//#define MGL_DEBUG // REMEMBER TO REMOVE THIS!
#define GL_ClearError() while (glGetError() != 0)
#ifdef MGL_DEBUG
#define GL_Call(x) GL_ClearError(); x; ASSERT(GL_LogCall(#x, __FILE__, __LINE__)) 
#else
#define GL_Call(x) x;
#endif

enum class BufferUsage
{
	STREAM_DRAW = 0x88E0,
	STREAM_READ,
	STREAM_COPY,
	STATIC_DRAW,
	STATIC_READ,
	STATIC_COPY,
	DYNAMIC_DRAW,
	DYNAMIC_READ,
	DYNAMIC_COPY
};

inline bool GL_LogCall(const char* functionName, const char* srcFile, const int line)
{
	while (uint32_t error = glGetError())
	{
		printf("[GL Error @ line %d]:\n\tFile: %s\n\tFunction: %s\n\tGL Error Code: 0x%X",
			line,
			srcFile,
			functionName,
			error);
		return false;
	}
	return true;
}

#endif // MGL_CONSTANTS_H