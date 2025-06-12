#pragma once
#include "Component.h"
#include "Subject.h"
#include <memory>
#include <vector>
#include "GridMovementComponent.h"
#include "PlayerState.h"

// Forward declarations
namespace yev
{
    class TransformComponent;
    class RenderComponent;
    class GameObject;
}

class Map;

// Player component that manages different states for Dig Dug player
class Player final : public yev::Component, public yev::Subject
{
public:
    Player(yev::GameObject* ownerObjectPtr, Map* map);
    ~Player() override;

    // Prevent copying
    Player(const Player& other) = delete;
    Player(Player&& other) = delete;
    Player& operator=(const Player& other) = delete;
    Player& operator=(Player&& other) = delete;

    // Core component methods
    void Update() override;
    void Render() const override;

    // State management
    void ChangeState(std::unique_ptr<PlayerState> newState);


    void BindPlayerInput();
    // Input handling
    void HandleMovementInput(GridMovementComponent::MovementDirection direction);
    void HandleDigInput();
    void HandleAttackInput();
    
    // Event handlers
    void TakeDamage(int damage = 1);
    void Kill();
    void AddScore(int points);

    // Properties access
    float GetMoveSpeed() const { return m_MoveSpeed; }
    int GetHealth() const { return m_Health; }
    bool IsAlive() const { return m_Health > 0 && !m_IsDying; }
    int GetScore() const { return m_Score; }
    
    // Getters for related components
    Map* GetMap() const { return m_Map; }
    GridMovementComponent* GetMovement() const;

    float GetAnimationTime() { return m_AnimationTime; };

private:
    // Components and references
    Map* m_Map{};
    yev::TransformComponent* m_Transform{};
    yev::RenderComponent* m_RenderComponent{};
    
    // Core properties
    int m_Health{3};
    float m_MoveSpeed{150.0f};
    int m_Score{0};
    bool m_IsDying{false};

    const float m_AnimationTime{ 0.1f };
    
    // State machine
    std::unique_ptr<PlayerState> m_CurrentState{};
    
    // Initialize player
    void InitializePlayer();
};