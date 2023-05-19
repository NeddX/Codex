#ifndef CODEX_NATIVE_API_H
#define CODEX_NATIVE_API_H

#if defined(_WIN32) || defined(__CYGWIN__)
#   define CODEX_EXPORT __declspec(dllexport)
#else
#   define CODEX_EXPORT __attribute__((visibility("default")))
#endif
#define ARRAY_COUNT(x) sizeof(x) / sizeof(x[0])

#include <Codex.h>
#include <cstdint>

Codex::Window* g_WindowInstance = nullptr;

#ifdef __cplusplus
extern "C" {
#endif
	// Anonymous namespace
	// TODO: Organize this please.
	namespace {

		struct Vector2f
		{
		public:
			float x, y;

		public:
			Vector2f(Codex::Vector2f vec)
			{
				x = vec.x;
				y = vec.y;
			}
		};

		// Renderer
		class Texture2D
		{
		public:
			const char* filePath;
			Codex::TextureProperties textureProperties;
		};

		// Scene
		class Sprite
		{
		public:
			Texture2D texture;
			Codex::Rectf srcRect;
		};

		// Components

		struct DescriptorTransformComponent
		{
		public:
			float position[3];
			float rotation[3];
			float scale[3];
			
		public:
			void ToNative(Codex::TransformComponent& c)
			{
				std::memcpy(Codex::ValuePtr(c.position),	position,	sizeof(float) * ARRAY_COUNT(position));
				std::memcpy(Codex::ValuePtr(c.rotation),	rotation,	sizeof(float) * ARRAY_COUNT(rotation));
				std::memcpy(Codex::ValuePtr(c.scale),		scale,		sizeof(float) * ARRAY_COUNT(scale));
			}
		};

		struct DescriptorTagComponent
		{
		public:
			const char* tag;
		
		public:
			Codex::TagComponent ToNative()
			{
				return Codex::TagComponent(tag);
			}
		};

		struct DescriptorSpriteRendererComponent
		{
		public:
			Codex::Vector4f colour;
			Sprite sprite;
			int32_t zIndex;

		public:
			void ToNative(Codex::SpriteRendererComponent& c)
			{
				// TODO: Convert to native type
			}
		};

		struct TileInfo
		{
			int32_t layer;
			Codex::Vector2f tilePos;
			Codex::Vector2f worldPos;
		};

		struct DescriptorTilemapComponent
		{
		public:
			const char* textureFilePath;
			Codex::Vector2f gridSize;
			Codex::Vector2f tileSize;
			int32_t layer;
			int32_t tileCount;

		public:
			/*
			Codex::TilemapComponent ToNative()
			{
				Codex::TilemapComponent c(
					Codex::Resources::Load<Codex::Texture2D>(textureFilePath), gridSize, tileSize);
				
				for (int i = 0; i < tileCount; ++i)
				{
					c.SetLayer(tiles[i].layer);
					c.AddTile(tiles[i].worldPos, tiles[i].tilePos);
				}

				return std::move(c);
			}*/
		};

		struct EntityDescriptor
		{
			DescriptorTransformComponent transform;
			DescriptorTagComponent tag;
			DescriptorSpriteRendererComponent spriteRenderer;
		};
	}

	// Window class
	CODEX_EXPORT void Init();
	CODEX_EXPORT void CreateWindow(const Codex::Window::Properties& proFperties, const void* nativeWindow = nullptr);
	CODEX_EXPORT void Destroy();
	CODEX_EXPORT void StartEngineThread();
	CODEX_EXPORT void Update();
	CODEX_EXPORT void ResizeViewport(int newWidth, int newHeight);
	CODEX_EXPORT Codex::Scene* GetCurrentScene();
	CODEX_EXPORT void ChangeScene(int sceneID);

	// Scene class
	CODEX_EXPORT int32_t CreateEntity(EntityDescriptor* desc);
	CODEX_EXPORT void RemoveEntity(int32_t id);
	CODEX_EXPORT void UpdateEntityFromDescriptor(int32_t id, EntityDescriptor* desc); // TODO: Obsolete function, remember to remove.
	CODEX_EXPORT void SetSelectedTileCoord(Codex::Vector2f newCoord);
	CODEX_EXPORT Codex::Vector2f GetSelectedTileCoord();
	CODEX_EXPORT void GetMousePositionInWorld(Codex::Vector2f& vec);

	// Entity Component System
	CODEX_EXPORT void AddSpriteRendererComponent(int32_t id, DescriptorSpriteRendererComponent* component);
	CODEX_EXPORT void AddTilemapComponent(int32_t id, DescriptorTilemapComponent* component, TileInfo* const tiles);

	CODEX_EXPORT void UpdateTransformComponent(int32_t id, DescriptorTransformComponent* c);
	CODEX_EXPORT void UpdateTagComponent(int32_t id, DescriptorTagComponent* c);
	CODEX_EXPORT void UpdateSpriteRendererComponent(int32_t id, DescriptorSpriteRendererComponent* c);
	CODEX_EXPORT void UpdateTilemapComponent(int32_t id, DescriptorTilemapComponent* c, TileInfo* const tiles);

#ifdef __cplusplus
}
#endif

#endif // CODEX_NATIVE_API_H