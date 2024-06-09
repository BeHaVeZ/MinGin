#include "pch.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}

    CheckCollisions();
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void dae::Scene::CheckCollisions()
{
    for (size_t i = 0; i < m_objects.size(); ++i)
    {
        auto collider1 = m_objects[i]->GetComponent<BoxCollider>();
        if (!collider1) continue;

        for (size_t j = i + 1; j < m_objects.size(); ++j)
        {
            auto collider2 = m_objects[j]->GetComponent<BoxCollider>();
            if (!collider2) continue;

            if (collider1->IsCollidingWith(*collider2))
            {
                collider1->ResolveCollision(*collider2);
            }
        }
    }
}

