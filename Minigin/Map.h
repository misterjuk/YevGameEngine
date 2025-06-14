#pragma once
#include "Component.h"
#include "Position.h"
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <glm.hpp> 



namespace yev
{
    class Texture2D;
    class GameObject;
    class TransformComponent;
    class Scene;
}

enum class TileType
{
    Empty,
    Earth,
    Rock,
    PlayerSpawn,
    EnemySpawn
};

class Enemy;
class Player;

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


    void SpawnEnemies(yev::Scene& scene);
    std::unique_ptr<yev::GameObject> CreateEnemyAt(const Position& position, int enemyType = 0);

    void SpawnPlayers(yev::Scene& scene);
    std::unique_ptr<yev::GameObject> CreatePlayerAt(const Position& position);

    glm::vec3 GridToWorldPosition(const Position& gridPos) const;
    Position WorldToGridPosition(const glm::vec3& worldPos) const;

    void Render() const override;

    void RegisterEnemy(Enemy* enemy);
    void UnregisterEnemy(Enemy* enemy);
    const std::vector<Enemy*>& GetAllEnemies() const { return m_Enemies; };

    void RegisterPlayer(Player* player);
    void UnregisterPlayer(Player* player);
    const std::vector<Player*>& GetAllPlayers() const { return m_Players; }


    void ClearLevel();
    void LoadLevel(yev::Scene& scene, int levelIndex);
    void LoadNextLevel();

private:

    void CreateUI(yev::Scene& scene);

    std::vector<std::vector<TileType>> m_Tiles;
    int m_Width{};
    int m_Height{};
    int m_TileSize{64};

	int m_ThisMapLevelIndex{0};

    bool m_UIInitialized{ false };


	const std::string m_Level1FilePath{ "../Data/Level1.txt" };
    const std::string m_Level2FilePath{ "../Data/Level2.txt" };
    const std::string m_Level3FilePath{ "../Data/Level3.txt" };

    std::vector<Enemy*> m_Enemies;
    std::vector<Player*> m_Players;

    const std::string m_TextureEarthPath{"Earth.png"};
    const std::string m_TextureRockPath{"Earth2.png"};
    
    const std::string m_TexturePookaPath{"Enemy.png"};
    const std::string m_TextureFygarPath{ "Enemy2.png" };

    const std::string m_TexturePlayerPath{ "Player.png" };

    std::unique_ptr<yev::GameObject> m_EarthObj{ nullptr };
    std::unique_ptr<yev::GameObject> m_RockObj{ nullptr };
};
