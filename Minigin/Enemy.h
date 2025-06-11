#pragma once
#include "Component.h"
#include "EnemyState.h"
#include "GridMovementComponent.h"
#include "Subject.h"
#include <memory>
#include <vector>

namespace yev
{
    class TransformComponent;
    class RenderComponent;
}

class Map;

// Enemy component that manages different states for Dig Dug-style enemies
class Enemy final : public yev::Component, public yev::Subject
{
public:
    enum class EnemyType
    {
        Pooka, // Regular enemy
        Fygar  // Fire-breathing enemy
    };
    
    Enemy(yev::GameObject* ownerObjectPtr, Map* map, EnemyType type = EnemyType::Pooka);
    ~Enemy() override = default;

    // Prevent copying
    Enemy(const Enemy& other) = delete;
    Enemy(Enemy&& other) = delete;
    Enemy& operator=(const Enemy& other) = delete;
    Enemy& operator=(Enemy&& other) = delete;

    // Core component methods
    void Update() override;
    void Render() const override;

    // State management
    void ChangeState(std::unique_ptr<EnemyState> newState);

    // Event handlers
    void TakeDamage(int damage = 1);
    void Kill();
    void Stun();

    // AI properties access
    float GetVisionRange() const { return m_VisionRange; }
    float GetMoveSpeed() const { return m_MoveSpeed; }
    int GetHealth() const { return m_Health; }
    bool IsAlive() const { return m_Health > 0; }
    EnemyType GetType() const { return m_Type; }
    
    // Getters for related components
    Map* GetMap() const { return m_Map; }
    GridMovementComponent* GetMovement() const;

private:
    // Components and references
    Map* m_Map{nullptr};
    yev::TransformComponent* m_Transform{nullptr};
    yev::RenderComponent* m_RenderComponent{nullptr};
    
    // Core properties
    int m_Health{2};
    float m_MoveSpeed{100.0f};
    float m_VisionRange{5.0f}; // In grid cells
    EnemyType m_Type{EnemyType::Pooka};
    
    // State machine
    std::unique_ptr<EnemyState> m_CurrentState{nullptr};

    // Player detection
    bool m_PlayerDetected{false};
    float m_PlayerDetectionTimer{0.0f};
    float m_PlayerDetectionInterval{0.5f}; // Check player detection every 0.5 seconds
    
    // Initialize AI behavior
    void InitializeAI();
};