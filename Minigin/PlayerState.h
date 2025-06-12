#pragma once
#include <memory>
#include "GridMovementComponent.h"
#include <string>

// Forward declarations
namespace yev { 
                class GameObject; 
                class RenderComponent; 
               }
class Map;  
class Player;

// Abstract base class for all player states
class PlayerState
{
public:
    virtual ~PlayerState() = default;

    // Core state functionality
    virtual void Enter(Player* player) = 0;
    virtual void Update(Player* player, float deltaTime) = 0;
    virtual void Exit(Player* player) = 0;

    // Handle events that might trigger state transitions
    virtual std::unique_ptr<PlayerState> HandleMovementInput(Player* player, GridMovementComponent::MovementDirection direction);
    virtual std::unique_ptr<PlayerState> HandleDigInput(Player* player);
    virtual std::unique_ptr<PlayerState> HandleAttackInput(Player* player);
    virtual std::unique_ptr<PlayerState> HandleDamaged(Player* player);
    virtual std::unique_ptr<PlayerState> HandleStateExpired(Player* player);

protected:
    bool CanMove(Player* player, GridMovementComponent::MovementDirection direction) const;
};

// Concrete player states
class PlayerIdleState : public PlayerState
{
public:
    void Enter(Player* player) override;
    void Update(Player* player, float deltaTime) override;
    void Exit(Player* player) override;
    
    std::unique_ptr<PlayerState> HandleMovementInput(Player* player, GridMovementComponent::MovementDirection direction) override;
    std::unique_ptr<PlayerState> HandleDigInput(Player* player) override;
    std::unique_ptr<PlayerState> HandleAttackInput(Player* player) override;
    
private:
    float m_IdleTime{0.0f};
};

class PlayerMovingState : public PlayerState
{
public:
    PlayerMovingState(GridMovementComponent::MovementDirection direction);
    
    void Enter(Player* player) override;
    void Update(Player* player, float deltaTime) override;
    void Exit(Player* player) override;
    
    std::unique_ptr<PlayerState> HandleMovementInput(Player* player, GridMovementComponent::MovementDirection direction) override;
    std::unique_ptr<PlayerState> HandleDigInput(Player* player) override;
    std::unique_ptr<PlayerState> HandleAttackInput(Player* player) override;
    std::unique_ptr<PlayerState> HandleStateExpired(Player* player) override;

private:
    GridMovementComponent::MovementDirection m_Direction{GridMovementComponent::MovementDirection::None};
    bool m_HasMoved{false};

    const std::string m_RunLeft1{ "Player/PlayerRunLeft1.png" };
    const std::string m_RunLeft2{ "Player/PlayerRunLeft2.png" };
    const std::string m_RunUp1{ "Player/PlayerRunUp1.png" };
    const std::string m_RunUp2{ "Player/PlayerRunUp2.png" };
    const std::string m_RunDown1{ "Player/PlayerRunDown1.png" };
    const std::string m_RunDown2{ "Player/PlayerRunDown2.png" };
    const std::string m_RunRight1{ "Player/PlayerRunRight1.png" };
    const std::string m_RunRight2{ "Player/PlayerRunRight2.png" };

    float m_CurrentAnimationTime{};

    yev::RenderComponent* playerRenderComponent;
};

class PlayerDiggingState : public PlayerState
{
public:
    void Enter(Player* player) override;
    void Update(Player* player, float deltaTime) override;
    void Exit(Player* player) override;
    
    std::unique_ptr<PlayerState> HandleStateExpired(Player* player) override;

private:
    float m_DigTimer{0.0f};
    float m_DigDuration{0.5f};
};

class PlayerAttackingState : public PlayerState
{
public:
    void Enter(Player* player) override;
    void Update(Player* player, float deltaTime) override;
    void Exit(Player* player) override;
    
    std::unique_ptr<PlayerState> HandleStateExpired(Player* player) override;

private:
    float m_AttackTimer{0.0f};
    float m_AttackDuration{1.0f};
    float m_AttackRange{3.0f}; // In grid cells
};

class PlayerDyingState : public PlayerState
{
public:
    void Enter(Player* player) override;
    void Update(Player* player, float deltaTime) override;
    void Exit(Player* player) override;

private:
    float m_DeathTimer{0.0f};
    float m_DeathAnimationDuration{2.0f};
    bool m_AnimationComplete{false};
};