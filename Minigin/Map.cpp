#include "Map.h"
#include <fstream>
#include <iostream>
#include "ResourceManager.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "TransformComponent.h"


Map::Map(yev::GameObject* ownerObjectPtr)
    : yev::Component(ownerObjectPtr),
    m_Width(0)
    , m_Height(0)
    , m_TileSize(64) // Default tile size for Dig Dug style
{
    // Create child GameObjects for rendering different tile types
    m_EarthObj = std::make_unique<yev::GameObject>();
    m_EarthObj->AddComponent<yev::TransformComponent>(m_EarthObj.get());
    m_EarthObj->GetComponent<yev::TransformComponent>()->SetScale(glm::vec3(8, 8, 1.0f));
    m_EarthObj->AddComponent<yev::RenderComponent>(m_EarthObj.get(), m_TextureEarthPath);

    m_RockObj = std::make_unique<yev::GameObject>();
    m_RockObj->AddComponent<yev::TransformComponent>(m_RockObj.get());
    m_RockObj->GetComponent<yev::TransformComponent>()->SetScale(glm::vec3(8, 8, 1.0f));
    m_RockObj->AddComponent<yev::RenderComponent>(m_RockObj.get(), m_TextureRockPath);

    // Set parent-child relationships to have proper transforms
    m_EarthObj->SetParent(ownerObjectPtr, true);
    m_RockObj->SetParent(ownerObjectPtr, true);
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