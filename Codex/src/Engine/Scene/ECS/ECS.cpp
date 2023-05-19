#include "ECS.h"

namespace Codex {
	/*
	Entity::Entity(const std::string& name) : m_Name(name)
	{
		std::fill(m_ComponentList.begin(), m_ComponentList.end(), nullptr);
	}

	Entity::~Entity()
	{
		for (const auto& c : m_Components) delete c;
	}

	void Entity::Start()
	{
		fmt::println("Entity initialized!");
	}

	void Entity::Update(float deltaTime)
	{
		std::for_each(m_Components.begin(),
			std::remove_if(m_Components.begin(), m_Components.end(),
				[](auto* c)
				{
					return !c->GetActive();
				}),
			[deltaTime](auto* c)
			{
				c->Update(deltaTime);
			});
	}

	void Entity::Render(float deltaTime, SpriteBatchRenderer* renderer)
	{
		std::for_each(m_Components.begin(),
			std::remove_if(m_Components.begin(), m_Components.end(),
				[](auto* c)
				{
					return !c->GetVisible();
				}),
			[deltaTime, renderer](auto* c)
			{
				c->Render(deltaTime, renderer);
			});
	}

	void Entity::Editor()
	{

	}
	*/
}