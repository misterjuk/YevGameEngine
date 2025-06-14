#include "Map.h"
#include <fstream>
#include <iostream>
#include "ResourceManager.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "GridMovementComponent.h"

#include "Scene.h"
#include "Player.h"
#include "Enemy.h"

#include "TextComponent.h"
#include "ScoreDisplayComponent.h"
#include "HealthDisplayComponent.h"
#include "FPSComponent.h"
#include "ScoreComponent.h"
#include "GameManager.h"
#include "HighScoreDisplayComponent.h"

Map::Map(yev::GameObject* ownerObjectPtr)
    : yev::Component(ownerObjectPtr),
    m_Width(0),
    m_Height(0),
    m_TileSize(64)
{
    m_EarthObj = std::make_unique<yev::GameObject>();
    m_EarthObj->AddComponent<yev::TransformComponent>(m_EarthObj.get());
    m_EarthObj->GetComponent<yev::TransformComponent>()->SetScale(glm::vec3(8, 8, 1.0f));
    m_EarthObj->AddComponent<yev::RenderComponent>(m_EarthObj.get(), m_TextureEarthPath);

    m_RockObj = std::make_unique<yev::GameObject>();
    m_RockObj->AddComponent<yev::TransformComponent>(m_RockObj.get());
    m_RockObj->GetComponent<yev::TransformComponent>()->SetScale(glm::vec3(8, 8, 1.0f));
    m_RockObj->AddComponent<yev::RenderComponent>(m_RockObj.get(), m_TextureRockPath);

    m_EarthObj->SetParent(ownerObjectPtr, true);
    m_RockObj->SetParent(ownerObjectPtr, true);

}

Enemy::~Enemy()
{
    if (m_Map && !IsAlive())
    {
        m_Map->UnregisterEnemy(this);
    }
}

bool Map::LoadFromFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open map file: " << filePath << std::endl;
        return false;
    }

    // Read map dimensions
    file >> m_Width >> m_Height;
    if (m_Width <= 0 || m_Height <= 0)
    {
        std::cerr << "Invalid map dimensions" << std::endl;
        return false;
    }

    // Initialize map with empty tiles
    m_Tiles.resize(m_Height, std::vector<TileType>(m_Width, TileType::Empty));

    // Read map data
    char tileChar;
    for (int y = 0; y < m_Height; ++y)
    {
        for (int x = 0; x < m_Width; ++x)
        {
            file >> tileChar;
            switch (tileChar)
            {
            case '.': m_Tiles[y][x] = TileType::Empty; break;
            case '#': m_Tiles[y][x] = TileType::Earth; break;
            case 'R': m_Tiles[y][x] = TileType::Rock; break;
            case 'P': m_Tiles[y][x] = TileType::PlayerSpawn; break;
            case 'E': m_Tiles[y][x] = TileType::EnemySpawn; break;
            default: m_Tiles[y][x] = TileType::Empty; break;
            }
        }
    }

    return true;
}

void Map::CreateEmptyMap(int width, int height)
{
    m_Width = width;
    m_Height = height;
    m_Tiles.resize(height, std::vector<TileType>(width, TileType::Earth));

    // Add border of rocks around the map
    for (int x = 0; x < width; ++x)
    {
        m_Tiles[0][x] = TileType::Rock;
        m_Tiles[height - 1][x] = TileType::Rock;
    }

    for (int y = 0; y < height; ++y)
    {
        m_Tiles[y][0] = TileType::Rock;
        m_Tiles[y][width - 1] = TileType::Rock;
    }
    
    // Add player spawn point
    m_Tiles[1][1] = TileType::PlayerSpawn;
    
    // Add some enemy spawn points
    m_Tiles[1][width-2] = TileType::EnemySpawn;
    m_Tiles[height-2][1] = TileType::EnemySpawn;
}

TileType Map::GetTileAt(int x, int y) const
{
    if (IsValidPosition(x, y))
        return m_Tiles[y][x];
    return TileType::Rock; // Return rock for out-of-bounds positions
}

void Map::SetTileAt(int x, int y, TileType type)
{
    if (IsValidPosition(x, y))
        m_Tiles[y][x] = type;
}

bool Map::IsValidPosition(int x, int y) const
{
    return x >= 0 && x < m_Width && y >= 0 && y < m_Height;
}

void Map::DigTunnel(int x, int y)
{
    if (IsValidPosition(x, y) && m_Tiles[y][x] == TileType::Earth)
        m_Tiles[y][x] = TileType::Empty;
}

Position Map::GetPlayerSpawnPosition() const
{
    for (int y = 0; y < m_Height; ++y)
    {
        for (int x = 0; x < m_Width; ++x)
        {
            if (m_Tiles[y][x] == TileType::PlayerSpawn)
                return { x, y };
        }
    }
    return { 1, 1 }; // Default position if none found
}

std::vector<Position> Map::GetEnemySpawnPositions() const
{
    std::vector<Position> positions;
    for (int y = 0; y < m_Height; ++y)
    {
        for (int x = 0; x < m_Width; ++x)
        {
            if (m_Tiles[y][x] == TileType::EnemySpawn)
                positions.push_back({ x, y });
        }
    }
    return positions;
}

void Map::SpawnEnemies(yev::Scene& scene)
{
    std::vector<Position> enemyPositions = GetEnemySpawnPositions();
    if (enemyPositions.empty())
    {
        std::cout << "No enemy spawn positions found in map." << std::endl;
        return;
    }

    // Create enemies at each spawn position
    int enemyCounter = 0;
    for (const auto& position : enemyPositions)
    {
        // Alternate between enemy types
        int enemyType = enemyCounter % 2;
        
        auto enemy = CreateEnemyAt(position, enemyType);

        scene.Add(std::move(enemy));
        enemyCounter++;
    }
    
    std::cout << "Spawned " << enemyCounter << " enemies" << std::endl;
}

std::unique_ptr<yev::GameObject> Map::CreateEnemyAt(const Position& position, int enemyType)
{
    auto enemyObj = std::make_unique<yev::GameObject>();
    
    enemyObj->AddComponent<yev::TransformComponent>(enemyObj.get());
    enemyObj->GetComponent<yev::TransformComponent>()->SetScale(glm::vec3(3.0f, 3.0f, 1.0f));
    
    enemyObj->AddComponent<yev::RenderComponent>(enemyObj.get());

    enemyObj->AddComponent<GridMovementComponent>(enemyObj.get(), this, false);
    enemyObj->GetComponent<GridMovementComponent>()->SetGridPosition(position.x, position.y);
    

    Enemy* enemyComponent = nullptr;
    if (enemyType == 0) // Pooka
    {
        enemyObj->GetComponent<yev::RenderComponent>()->SetTexture(m_TexturePookaPath);
        enemyComponent = enemyObj->AddComponent<Enemy>(enemyObj.get(), this, Enemy::EnemyType::Pooka);
    }
    else // Fygar
    {
        enemyObj->GetComponent<yev::RenderComponent>()->SetTexture(m_TextureFygarPath);
        enemyComponent = enemyObj->AddComponent<Enemy>(enemyObj.get(), this, Enemy::EnemyType::Fygar);
    }

    if (enemyComponent)
    {
        RegisterEnemy(enemyComponent);
    }
     
    return enemyObj;
}

//can be modified to spawn multiple players if needed
void Map::SpawnPlayers(yev::Scene& scene)
{
    Position playerPosition = GetPlayerSpawnPosition();
 
    auto player = CreatePlayerAt(playerPosition);


    if(!m_UIInitialized)
    {
        
    auto font = yev::ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
    //BAD WORKAROUND
    auto scoreDisplay = std::make_unique<yev::GameObject>();
    scoreDisplay->AddComponent<yev::TransformComponent>(scoreDisplay.get());
    scoreDisplay->GetComponent<yev::TransformComponent>()->SetLocalPosition(1050, 400, 0);
    scoreDisplay->AddComponent<yev::TextComponent>(scoreDisplay.get(), "Score", font);
    scoreDisplay->AddComponent<ScoreDisplayComponent>(scoreDisplay.get(), scoreDisplay->GetComponent<yev::TextComponent>());

    player->GetComponent<ScoreComponent>()->AddObserver(scoreDisplay->GetComponent<ScoreDisplayComponent>());


    auto highscoreDisplay = std::make_unique<yev::GameObject>();
    highscoreDisplay->AddComponent<yev::TransformComponent>(highscoreDisplay.get());
    highscoreDisplay->GetComponent<yev::TransformComponent>()->SetLocalPosition(1050, 500, 0);
    highscoreDisplay->AddComponent<yev::TextComponent>(highscoreDisplay.get(), "HighScore", font);
    highscoreDisplay->AddComponent<HighScoreDisplayComponent>(highscoreDisplay.get(), highscoreDisplay->GetComponent<yev::TextComponent>());

    player->GetComponent<ScoreComponent>()->AddObserver(highscoreDisplay->GetComponent<HighScoreDisplayComponent>());

    //BAD WORKAROUND
    auto healthDisplay = std::make_unique<yev::GameObject>();
    healthDisplay->AddComponent<yev::TransformComponent>(healthDisplay.get());
    healthDisplay->GetComponent<yev::TransformComponent>()->SetLocalPosition(1050, 600, 0);
    healthDisplay->AddComponent<yev::TextComponent>(healthDisplay.get(), "Health", font);
    healthDisplay->AddComponent<HealthDisplayComponent>(healthDisplay.get(), healthDisplay->GetComponent<yev::TextComponent>());

    scene.Add(std::move(scoreDisplay));
    scene.Add(std::move(highscoreDisplay));
    scene.Add(std::move(healthDisplay));

    }
    else {
        //BAD WORKAROUND
        auto& activeScene = yev::SceneManager::GetInstance().GetActiveScene();

        for (const auto& obj : activeScene.GetObjects())
        {
            if (obj->HasComponent<ScoreDisplayComponent>())
            {
                player->GetComponent<ScoreComponent>()->AddObserver(obj->GetComponent<ScoreDisplayComponent>());
                //BAD WORKAROUND
                obj->GetComponent<ScoreDisplayComponent>()->ForceScoreUpdate();
                //break;
				//should be return or something like that to stop the loop, but this is a workaround
            }
            else if (obj->HasComponent<HighScoreDisplayComponent>())
            {
                player->GetComponent<ScoreComponent>()->AddObserver(obj->GetComponent<HighScoreDisplayComponent>());
                //BAD WORKAROUND
                obj->GetComponent<HighScoreDisplayComponent>()->ForceScoreUpdate();
                //break;
                //should be return or something like that to stop the loop, but this is a workaround
            }
        }
    }
   
    scene.Add(std::move(player));

   

    std::cout << "Spawned Player" << std::endl;
}

std::unique_ptr<yev::GameObject> Map::CreatePlayerAt(const Position& position)
{
    auto playerObj = std::make_unique<yev::GameObject>();

    playerObj->AddComponent<yev::TransformComponent>(playerObj.get());
    playerObj->GetComponent<yev::TransformComponent>()->SetScale(glm::vec3(3.0f, 3.0f, 1.0f));

   
    playerObj->AddComponent<yev::RenderComponent>(playerObj.get());
    playerObj->GetComponent<yev::RenderComponent>()->SetTexture(m_TexturePlayerPath);

    playerObj->AddComponent<GridMovementComponent>(playerObj.get(), this, true);
    playerObj->GetComponent<GridMovementComponent>()->SetGridPosition(position.x, position.y);

	playerObj->AddComponent<ScoreComponent>(playerObj.get());

    int savedScore = GameManager::GetInstance().GetPlayerScore();
    if (savedScore > 0)
    {
        playerObj->GetComponent<ScoreComponent>()->SetScore(savedScore);
    }

    playerObj->AddComponent<Player>(playerObj.get(), this);



    return playerObj;
}

glm::vec3 Map::GridToWorldPosition(const Position& gridPos) const
{
    float x = static_cast<float>(gridPos.x * m_TileSize);
    float y = static_cast<float>(gridPos.y * m_TileSize);
    return glm::vec3(x, y, 0.0f);
}

Position Map::WorldToGridPosition(const glm::vec3& worldPos) const
{
    int x = static_cast<int>(worldPos.x) / m_TileSize;
    int y = static_cast<int>(worldPos.y) / m_TileSize;
    return Position{ x, y };
}

void Map::Render() const
{
    for (int y = 0; y < m_Height; ++y)
    {
        for (int x = 0; x < m_Width; ++x)
        {
            float posX = static_cast<float>(x * m_TileSize);
            float posY = static_cast<float>(y * m_TileSize);

            switch (m_Tiles[y][x])
            {
            case TileType::Earth:
                if (m_EarthObj)
                {
                    // Set position for this tile
                    auto* transform = m_EarthObj->GetComponent<yev::TransformComponent>();
                    if (transform)
                    {
                        transform->SetLocalPosition(posX, posY, 0);
                        m_EarthObj->GetComponent<yev::RenderComponent>()->Render();
                    }
                }
                break;

            case TileType::Rock:
                if (m_RockObj)
                {
                    // Set position for this tile
                    auto* transform = m_RockObj->GetComponent<yev::TransformComponent>();
                    if (transform)
                    {
                        transform->SetLocalPosition(posX, posY, 0);
                        m_RockObj->GetComponent<yev::RenderComponent>()->Render();
                    }
                }
                break;

                // Other tile types aren't rendered or use special handling
            default:
                break;
            }
        }
    }
}

void Map::RegisterEnemy(Enemy* enemy)
{
    if (enemy)
    {
        auto it = std::find(m_Enemies.begin(), m_Enemies.end(), enemy);
        if (it == m_Enemies.end())
        {
            m_Enemies.push_back(enemy);
        }
    }
}
void Map::UnregisterEnemy(Enemy* enemy)
{
    if (enemy && !m_Enemies.empty() && !enemy->IsAlive())
    {
        auto it = std::find(m_Enemies.begin(), m_Enemies.end(), enemy);
        if (it != m_Enemies.end())
        {
            m_Enemies.erase(it);

            
            if (m_Enemies.empty())
            {
                std::cout << "All enemies defeated! Loading next level..." << std::endl;
                ClearLevel();
                LoadNextLevel();
            }
        }
    }
}

void Map::RegisterPlayer(Player* player)
{
    if (player)
    {
        auto it = std::find(m_Players.begin(), m_Players.end(), player);
        if (it == m_Players.end())
        {
            m_Players.push_back(player);
        }
    }
}

void Map::UnregisterPlayer(Player* player)
{
    if (player && !m_Players.empty())
    {
        auto it = std::find(m_Players.begin(), m_Players.end(), player);
        if (it != m_Players.end())
        {
            m_Players.erase(it);
        }
    }
}

void Map::ClearLevel()
{
	
    if (!m_Players.empty()) {
        Player* player = m_Players[0]; // Assuming single player for simplicity
        if (player && player->GetOwner() && player->GetOwner()->HasComponent<ScoreComponent>()) {
            player->GetOwner()->GetComponent<ScoreComponent>()->SetScore(GameManager::GetInstance().GetPlayerScore());
          
        }
    }

    for (auto* enemy : m_Enemies)
    {
        if (enemy && enemy->GetOwner())
        {
            enemy->GetOwner()->MarkForDeletion();
        }
    }
    // Clear the enemies list
    m_Enemies.clear();

    for (auto* player : m_Players)
    {
        if (player && player->GetOwner())
        {
            player->GetOwner()->MarkForDeletion();
        }
    }

    m_Players.clear();

    // Clear the tile map
    for (auto& row : m_Tiles)
    {
        row.clear();
    }
    m_Tiles.clear();

    // Reset dimensions
    m_Width = 0;
    m_Height = 0;
}

void Map::LoadNextLevel()
{
   

	int nextLevelIndex = m_ThisMapLevelIndex + 1;

    if (nextLevelIndex == 4) {
        yev::SceneManager::GetInstance().SetActiveScene("EndScreen");
        return;
    }

	std::string nextLevelName = "Level" + nextLevelIndex;
    if (yev::SceneManager::GetInstance().SetActiveScene(nextLevelName))
    {

        auto& activeScene = yev::SceneManager::GetInstance().GetActiveScene();

        for (const auto& obj : activeScene.GetObjects())
        {
            if (obj->HasComponent<Map>())
            {
                obj->GetComponent<Map>()->LoadLevel(activeScene, m_ThisMapLevelIndex+1);
                break;
            }
        }

        yev::SceneManager::GetInstance().SetActiveScene(&activeScene);

        return;
    }
    else {
        auto& scene = yev::SceneManager::GetInstance().CreateScene(nextLevelName);

        auto map = std::make_unique<yev::GameObject>();
        map->AddComponent<yev::TransformComponent>(map.get());
        map->GetComponent<yev::TransformComponent>()->SetLocalPosition(0, 0, -5);
        map->AddComponent<Map>(map.get());
        map->GetComponent<Map>()->LoadLevel(scene, nextLevelIndex);

        scene.Add(std::move(map));

        auto& sceneManager = yev::SceneManager::GetInstance();
        sceneManager.SetActiveScene(&scene);
    }
    
}

void Map::LoadLevel(yev::Scene& scene, int levelIndex)
{
    ClearLevel();

    m_ThisMapLevelIndex = levelIndex;
    GameManager::GetInstance().SetCurrentLevel(levelIndex);

    bool success{};
    switch (m_ThisMapLevelIndex)
    {
    case 1:
        success = LoadFromFile(m_Level1FilePath);
        break;
	case 2:
        success = LoadFromFile(m_Level2FilePath);
		break;
    case 3:
        success = LoadFromFile(m_Level3FilePath);
		break;
    }
  
      
    if (!success)
    {
        std::cerr << "Failed to load next level" << std::endl;
        return;
    }

    // Spawn enemies and players in the new scene
    SpawnEnemies(scene);
    SpawnPlayers(scene);

    if(!m_UIInitialized)
    CreateUI(scene);
 
    std::cout << "Level " << m_ThisMapLevelIndex << " loaded successfully!" << std::endl;
}

void Map::CreateUI(yev::Scene& scene)
{
    // Add UI elements (based on Main.cpp's load function)
    auto hugefont = yev::ResourceManager::GetInstance().LoadFont("Lingua.otf", 60);
    auto font = yev::ResourceManager::GetInstance().LoadFont("Lingua.otf", 40);


    auto digdug = std::make_unique<yev::GameObject>();
    digdug->AddComponent<yev::TransformComponent>(digdug.get());
    digdug->GetComponent<yev::TransformComponent>()->SetLocalPosition(1050, 100, 0);
    digdug->AddComponent<yev::TextComponent>(digdug.get(), "DigDug", hugefont);
   
  

    // Add FPS counter
    auto fps = std::make_unique<yev::GameObject>();
    fps->AddComponent<yev::TransformComponent>(fps.get());
    fps->GetComponent<yev::TransformComponent>()->SetLocalPosition(1050, 200, 0);
    fps->AddComponent<yev::TextComponent>(fps.get(), font);
    fps->AddComponent<yev::FPSComponent>(fps.get());

    auto levelname = std::make_unique<yev::GameObject>();

	std::string name = "Level" + std::to_string(m_ThisMapLevelIndex);
    levelname->AddComponent<yev::TransformComponent>(levelname.get());
    levelname->GetComponent<yev::TransformComponent>()->SetLocalPosition(1050, 300, 0);
    levelname->AddComponent<yev::TextComponent>(levelname.get(), name, font);

    // Add all objects to the new scene
    scene.Add(std::move(levelname));
	scene.Add(std::move(digdug));

    scene.Add(std::move(fps));

	m_UIInitialized = true;
}