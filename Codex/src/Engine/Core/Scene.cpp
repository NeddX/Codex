#include "Scene.h"

namespace codex {
    Scene::Scene(Renderer* renderer, i32 width, i32 height) :
        m_Width(width), m_Height(height), m_Renderer(renderer)
    {
        m_Running = false;
        m_Camera = std::make_unique<Camera>(width, height);
        //m_ActiveEntity = nullptr;
    }

    /*
    void Scene::AddEntity(Entity* entity)
    {
        m_Entities.push_back(entity);
        if (m_Running) entity->Start();
    }
    */

    void Scene::Init()
    {
        this->Start();
        /*for (const auto& ent : m_Entities)
            ent->Start();*/
        m_Running = true;
    }
}
