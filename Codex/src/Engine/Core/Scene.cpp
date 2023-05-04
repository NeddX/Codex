#include "Scene.h"

namespace Codex
{
    Scene::Scene(Renderer* renderer, int width, int height) : 
        m_Renderer(renderer), m_Width(width), m_Height(height)
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
    /*
    void Scene::SceneEditor()
    {
        if (m_ActiveEntity)
        {
            // begin inspector
            m_ActiveEntity->Editor();
            // end inspector
        }

        Editor();
    }
    void Scene::Editor()
    {

    }*/
}
