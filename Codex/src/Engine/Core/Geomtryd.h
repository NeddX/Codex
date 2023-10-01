#ifndef CODEX_CORE_GEOMETRY_DEFS_H
#define CODEX_CORE_GEOMETRY_DEFS_H

#include <sdafx.h>

namespace Codex {
	using Matrix4f = glm::mat4;
	using Matrix2f = glm::mat2;
	using Vector2f = glm::vec2;
	using Vector3f = glm::vec3;
	using Vector4f = glm::vec4;
	using Rect = mgl::Rect;
	using Rectf = mgl::Rectf;

	// TODO: This looks retarded, I am sure there's a better way to accomplish this
	// using templates.
	inline f32* ValuePtr(Vector3f vector) { return glm::value_ptr(vector); }
	inline f32* ValuePtr(Vector2f vector) { return glm::value_ptr(vector); }
}

namespace std {
	template<>
	struct hash<Codex::Vector2f>
	{
		Codex::usize operator()(const Codex::Vector2f& vec) const
		{
			return hash<Codex::f32>()(vec.x) ^ hash<Codex::f32>()(vec.y);
		}
	};

	template<>
	struct hash<Codex::Vector3f>
	{
		Codex::usize operator()(const Codex::Vector3f& vec) const
		{
			return hash<Codex::f32>()(vec.x) ^ hash<Codex::f32>()(vec.y) ^ hash<Codex::f32>()(vec.z);
		}
	};

	template<>
	struct hash<Codex::Vector4f>
	{
		Codex::usize operator()(const Codex::Vector4f& vec) const
		{
			return hash<Codex::f32>()(vec.x) ^ hash<Codex::f32>()(vec.y) ^ hash<Codex::f32>()(vec.z) ^ hash<Codex::f32>()(vec.w);
		}
	};

	template<>
	struct hash<Codex::Rectf>
	{
		Codex::usize operator()(const Codex::Rectf& rect) const
		{
			return hash<Codex::f32>()(rect.x) ^ hash<Codex::f32>()(rect.y) ^ hash<Codex::f32>()(rect.w) ^ hash<Codex::f32>()(rect.h);
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
