#pragma once
#include <memory>
#include "GridMovementComponent.h" 

namespace yev { class GameObject; }
class Map;
class Enemy;

// Abstract base class for all enemy states
class EnemyState
{
public:
    virtual ~EnemyState() = default;

    // Core state functionality
    virtual void Enter(Enemy* enemy) = 0;
    virtual void Update(Enemy* enemy, float deltaTime) = 0;
    virtual void Exit(Enemy* enemy) = 0;

    // Handle events that might trigger state transitions
    virtual std::unique_ptr<EnemyState> HandlePlayerSeen(Enemy* enemy);
    virtual std::unique_ptr<EnemyState> HandlePlayerLost(Enemy* enemy);
    virtual std::unique_ptr<EnemyState> HandleDamaged(Enemy* enemy);
    virtual std::unique_ptr<EnemyState> HandleStateExpired(Enemy* enemy);

protected:
    // Helper for derived states to check if player is in line of sight
    bool IsPlayerInLineOfSight(Enemy* enemy, Map* map, int visionRange) const;
    
    // Helper to find next valid direction for movement
    GridMovementComponent::MovementDirection FindValidDirection(Enemy* enemy, Map* map) const;
};

// Concrete enemy states
class EnemyIdleState : public EnemyState
{
public:
    void Enter(Enemy* enemy) override;
    void Update(Enemy* enemy, float deltaTime) override;
    void Exit(Enemy* enemy) override;
    
    std::unique_ptr<EnemyState> HandlePlayerSeen(Enemy* enemy) override;
    std::unique_ptr<EnemyState> HandleStateExpired(Enemy* enemy) override;
private:
    float m_IdleTime{};
    float m_MaxIdleTime{2.0f};
};

class EnemyPatrolState : public EnemyState
{
public:
    void Enter(Enemy* enemy) override;
    void Update(Enemy* enemy, float deltaTime) override;
    void Exit(Enemy* enemy) override;
    
    std::unique_ptr<EnemyState> HandlePlayerSeen(Enemy* enemy) override;

private:
    GridMovementComponent::MovementDirection m_CurrentDirection{GridMovementComponent::MovementDirection::None};
    float m_DirectionTimer{};
    float m_DirectionChangeInterval{3.0f};
};

class EnemyChaseState : public EnemyState
{
public:
    void Enter(Enemy* enemy) override;
    void Update(Enemy* enemy, float deltaTime) override;
    void Exit(Enemy* enemy) override;
    
    std::unique_ptr<EnemyState> HandlePlayerLost(Enemy* enemy) override;
    std::unique_ptr<EnemyState> HandleDamaged(Enemy* enemy) override;

private:
    float m_PathUpdateTimer{};
    float m_PathUpdateInterval{1.0f};
};

class EnemyStunnedState : public EnemyState
{
public:
    void Enter(Enemy* enemy) override;
    void Update(Enemy* enemy, float deltaTime) override;
    void Exit(Enemy* enemy) override;
    
    std::unique_ptr<EnemyState> HandleStateExpired(Enemy* enemy) override;

private:
    float m_StunTimer{};
    float m_StunDuration{3.0f};
};

class EnemyDeathState : public EnemyState
{
public:
    void Enter(Enemy* enemy) override;
    void Update(Enemy* enemy, float deltaTime) override;
    void Exit(Enemy* enemy) override;

private:
    float m_DeathTimer{};
    float m_DeathAnimationDuration{2.0f};
    bool m_AnimationComplete{false};
};