#include "GameManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Map.h"
#include <iostream>
#include "TransformComponent.h"


bool GameManager::SkipToLevel(int levelNumber)
{
    if (levelNumber < 1 || levelNumber > 4) // Assuming you have 3 levels
    {
        std::cout << "Invalid level number: " << levelNumber << std::endl;
        return false;
    }
    else if(levelNumber == 4)
    {
       yev::SceneManager::GetInstance().SetActiveScene("EndScreen");
       m_CurrentLevel = 0;
       return true;
	}

    if (m_CurrentLevel == levelNumber)
    {
        std::cout << "Already on level " << levelNumber << std::endl;
        return false;
    }

    m_CurrentLevel = levelNumber;

    // Create the new scene for the level
    std::string levelName = "Level" + std::to_string(m_CurrentLevel);

    if (yev::SceneManager::GetInstance().SetActiveScene(levelName))
    {
        
        auto& activeScene = yev::SceneManager::GetInstance().GetActiveScene();

        for (const auto& obj : activeScene.GetObjects())
        {
            if (obj->HasComponent<Map>())
            {
                obj->GetComponent<Map>()->LoadLevel(activeScene, m_CurrentLevel);
                break;
            }
        }
		yev::SceneManager::GetInstance().SetActiveScene(&activeScene);

        return true;
	}
    else 
    {
        auto& scene = yev::SceneManager::GetInstance().CreateScene(levelName);

        // Create a map object for the new scene
        auto mapObj = std::make_unique<yev::GameObject>();
        mapObj->AddComponent<yev::TransformComponent>(mapObj.get());
        mapObj->GetComponent<yev::TransformComponent>()->SetLocalPosition(0, 0, -5);
        mapObj->AddComponent<Map>(mapObj.get());

        // Load the level but pass the current score to be set on the new player
        mapObj->GetComponent<Map>()->LoadLevel(scene, m_CurrentLevel);

        scene.Add(std::move(mapObj));

        // Set the new scene as active
        yev::SceneManager::GetInstance().SetActiveScene(&scene);

        return true;
    }
   
}