#ifndef CODEX_CORE_GEOMETRY_DEFS_H
#define CODEX_CORE_GEOMETRY_DEFS_H

#include <sdafx.h>

namespace Codex {
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

namespace std {
	template<>
	struct hash<Codex::Vector2f>
	{
		size_t operator()(const Codex::Vector2f& vec) const 
		{
			return hash<float>()(vec.x) ^ hash<float>()(vec.y);
		}
	};

	template<>
	struct hash<Codex::Vector3f>
	{
		size_t operator()(const Codex::Vector3f& vec) const
		{
			return hash<float>()(vec.x) ^ hash<float>()(vec.y) ^ hash<float>()(vec.z);
		}
	};

	template<>
	struct hash<Codex::Vector4f>
	{
		size_t operator()(const Codex::Vector4f& vec) const
		{
			return hash<float>()(vec.x) ^ hash<float>()(vec.y) ^ hash<float>()(vec.z) ^ hash<float>()(vec.w);
		}
	};

	template<>
	struct hash<Codex::Rectf>
	{
		size_t operator()(const Codex::Rectf& rect) const
		{
			return hash<float>()(rect.x) ^ hash<float>()(rect.y) ^ hash<float>()(rect.w) ^ hash<float>()(rect.h);
		}
	};
}

namespace fmt {
	template<>
	struct formatter<Codex::Vector2f>
	{
		template<typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template<typename FormatContext>
		auto format(const Codex::Vector2f& vec, FormatContext& ctx) 
		{ 
			return format_to(ctx.out(), "({}, {})", vec.x, vec.y); 
		}
	};


	template<>
	struct formatter<Codex::Vector3f>
	{
		template<typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template<typename FormatContext>
		auto format(const Codex::Vector3f& vec, FormatContext& ctx)
		{
			return format_to(ctx.out(), "({}, {}, {})", vec.x, vec.y, vec.z);
		}
	};


	template<>
	struct formatter<Codex::Vector4f>
	{
		template<typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template<typename FormatContext>
		auto format(const Codex::Vector4f& vec, FormatContext& ctx)
		{
			return format_to(ctx.out(), "({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w);
		}
	};


	template<>
	struct formatter<Codex::Rectf>
	{
		template<typename ParseContext>
		constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

		template<typename FormatContext>
		auto format(const Codex::Rectf& vec, FormatContext& ctx)
		{
			return format_to(ctx.out(), "({}, {}, {}, {})", vec.x, vec.y, vec.w, vec.h);
		}
	};
}

#endif // CODEX_CORE_GEOMETRY_DEFS_H