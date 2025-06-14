#include "SceneManager.h"
#include "Scene.h"
#include <iostream>

void yev::SceneManager::Update()
{
    if (m_activeScene)
    {
        // Only update the active scene
        m_activeScene->Update();
    }
    else
    {
        // Fall back to updating all scenes if no active scene is set
        for (auto& scene : m_scenes)
        {
            scene->Update();
        }
    }
}

void yev::SceneManager::FixedUpdate()

{

}

void yev::SceneManager::Render()
{
    if (m_activeScene)
    {
        // Only render the active scene
        m_activeScene->Render();
    }
    else
    {
        // Fall back to rendering all scenes if no active scene is set
        for (const auto& scene : m_scenes)
        {
            scene->Render();
        }
    }
}

yev::Scene& yev::SceneManager::CreateScene(const std::string& name)
{
    const auto& scene = std::shared_ptr<Scene>(new Scene(name));
    m_scenes.push_back(scene);

    // Set as active scene if this is the first scene
    if (m_scenes.size() == 1 || m_activeScene == nullptr)
    {
        m_activeScene = scene.get();
    }

    return *scene;
}

bool yev::SceneManager::SetActiveScene(Scene* scene)
{
    if (scene)
    {
        // Verify that the scene exists in our scenes collection
        auto it = std::find_if(m_scenes.begin(), m_scenes.end(),
            [scene](const std::shared_ptr<Scene>& scenePtr) {
                return scenePtr.get() == scene;
            });

        if (it != m_scenes.end())
        {
            m_activeScene = scene;
            return true;
        }
        else
        {
            // Scene not found in our collection
            std::cerr << "Warning: Attempting to set an unknown scene as active" << std::endl;
            return false;
        }
    }
    else
    {
        m_activeScene = nullptr;
        return false;
    }
}

bool yev::SceneManager::SetActiveScene(const std::string& name)
{
    auto it = std::find_if(m_scenes.begin(), m_scenes.end(),
        [&name](const std::shared_ptr<Scene>& scene) {
            return scene->GetName() == name;
        });

    if (it != m_scenes.end())
    {
        m_activeScene = it->get();
        std::cout << "Active scene changed to: " << name << std::endl;
		return true;
    }
    else
    {
		return false;
    }
}