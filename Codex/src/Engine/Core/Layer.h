#ifndef CODEX_CORE_LAYER_H
#define CODEX_CORE_LAYER_H

namespace Codex {
	// Forward decleartions
	class Scene;

	class Layer
	{
	protected:
		const char* m_Name;

	public:
		Layer(const char* name = "Layer")
		{
			m_Name = name;
		}
		virtual ~Layer() = default;

	public:
		virtual void OnAttach()										= 0;
		virtual void OnDetach()										= 0;
		virtual void Update(float deltaTime, Scene* currentScene)	= 0;
		virtual void Render(float deltaTime)						= 0;
		virtual void OnEvent()										= 0;

	public:
		const char* GetName() const { return m_Name; }
	};
}

#endif // CODEX_CORE_LAYER_H