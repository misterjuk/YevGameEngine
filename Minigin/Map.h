#pragma once
#include "Component.h"
#include <vector>
#include <string>
#include <memory>

#include <glm.hpp> 

namespace yev
{
    class Texture2D;
    class GameObject;
    class TransformComponent;
}

enum class TileType
{
    Empty,
    Earth,
    Rock,
    PlayerSpawn,
    EnemySpawn
};

struct Position
{
    int x;
    int y;
    bool operator==(const Position& other) const
    {
        return x == other.x && y == other.y;
    }
};

class Map final : public yev::Component
{
public:
    Map(yev::GameObject* ownerObjectPtr);
    ~Map() = default;

    Map(const Map& other) = delete;
    Map(Map&& other) = delete;
    Map& operator=(const Map& other) = delete;
    Map& operator=(Map&& other) = delete;

    bool LoadFromFile(const std::string& filePath);
    void CreateEmptyMap(int width, int height);

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    int GetTileSize() const { return m_TileSize; }
    void SetTileSize(int tileSize) { m_TileSize = tileSize; }

    TileType GetTileAt(int x, int y) const;
    void SetTileAt(int x, int y, TileType type);
    bool IsValidPosition(int x, int y) const;

    void DigTunnel(int x, int y);

    Position GetPlayerSpawnPosition() const;
    std::vector<Position> GetEnemySpawnPositions() const;

    // Get world position from grid position
    glm::vec3 GridToWorldPosition(const Position& gridPos) const;
    // Get grid position from world position
    Position WorldToGridPosition(const glm::vec3& worldPos) const;

    // Rendering
    void Render() const override;

private:
    std::vector<std::vector<TileType>> m_Tiles;
    int m_Width;
    int m_Height;
    int m_TileSize;

    // Textures for different tile types
    const std::string m_TextureEarthPath{"Earth.png"};
    const std::string m_TextureRockPath{ "Earth2.png" };


    // Components used for rendering
    std::unique_ptr<yev::GameObject> m_EarthObj{ nullptr };
    std::unique_ptr<yev::GameObject> m_RockObj{ nullptr };
};
