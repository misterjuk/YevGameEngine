#include "Scene.h"

#include <algorithm>
#include "TransformComponent.h"

using namespace yev;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::unique_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update()
{
    for (auto& object : m_objects)
    {
        if (!object->IsMarkedForDeletion())
        {
            object->Update();
        }
    }

    // Perform actual removal at the end
    m_objects.erase(
        std::remove_if(m_objects.begin(), m_objects.end(),
            [](const std::unique_ptr<GameObject>& obj) { return obj->IsMarkedForDeletion(); }),
        m_objects.end());
}

//TODO recalculating z coord every frame is bad should change it
void Scene::Render() const
{

   
    // Create a sorted copy of object pointers for rendering based on z-coordinate
    std::vector<GameObject*> sortedObjects;
    sortedObjects.reserve(m_objects.size());
    
    for (const auto& object : m_objects)
    {
        sortedObjects.push_back(object.get());
    }
    
    
    // Sort objects by z-coordinate (ascending order)
    // Objects with smaller z will be rendered first, larger z will be rendered later (on top)
    std::sort(sortedObjects.begin(), sortedObjects.end(), 
        [](const GameObject* a, const GameObject* b) {
            // If object has no transform component, default to lowest z value
            auto* transformA = a->GetComponent<yev::TransformComponent>();
            auto* transformB = b->GetComponent<yev::TransformComponent>();
            
            float zPosA = transformA ? transformA->GetWorldPosition().z : -FLT_MAX;
            float zPosB = transformB ? transformB->GetWorldPosition().z : -FLT_MAX;
            
            return zPosA < zPosB;
        });
    
    // Render objects in the sorted order
    for (const auto& object : sortedObjects)
    {
        object->Render();
    }
}

