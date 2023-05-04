#ifndef CODEX_CORE_GEOMETRY_DEFS_H
#define CODEX_CORE_GEOMETRY_DEFS_H

#include <sdafx.h>

namespace Codex
{
	typedef glm::mat4 Matrix4f;
	typedef glm::mat2 Matrix2f;
	typedef glm::vec2 Vector2f;
	typedef glm::vec3 Vector3f;
	typedef glm::vec4 Vector4f;
	typedef mgl::Rect Rect;
	typedef mgl::Rectf Rectf;

	// TODO: This looks retarded, I am sure there's a better way to accomplish this
	// using templates.
	inline float* ValuePtr(Vector3f vector) { return glm::value_ptr(vector); }
	inline float* ValuePtr(Vector2f vector) { return glm::value_ptr(vector); }
}

#endif // CODEX_CORE_GEOMETRY_DEFS_H