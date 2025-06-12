#include "EnemyState.h"
#include "Enemy.h"
#include "GameObject.h"
#include "GridMovementComponent.h"
#include "Map.h"
#include "RenderComponent.h"
#include "Time.h"
#include <iostream>

std::unique_ptr<EnemyState> EnemyState::HandlePlayerSeen(Enemy*)
{
    return nullptr;
}

std::unique_ptr<EnemyState> EnemyState::HandlePlayerLost(Enemy*)
{
    return nullptr;
}

std::unique_ptr<EnemyState> EnemyState::HandleDamaged(Enemy*)
{
    return std::make_unique<EnemyStunnedState>();
}

std::unique_ptr<EnemyState> EnemyState::HandleStateExpired(Enemy*)
{
    return nullptr;
}

bool EnemyState::IsPlayerInLineOfSight(Enemy* enemy, Map* map, int visionRange) const
{
    if (!enemy || !map)
        return false;
        
    auto movement = enemy->GetMovement();
    if (!movement)
        return false;
        
    visionRange = 0;
    // Simple implementation: would need to be expanded for actual game
    // For now, this is a placeholder that would check map for clear line of sight
    
    Position enemyPos = movement->GetGridPosition();
    // Would need a way to get player position for actual implementation
    // Position playerPos = FindClosestPlayer(enemyPos);
    
    return false; // Placeholder
}

GridMovementComponent::MovementDirection EnemyState::FindValidDirection(Enemy* enemy, Map* map) const
{
    if (!enemy || !map)
        return GridMovementComponent::MovementDirection::None;
        
    auto movement = enemy->GetMovement();
    if (!movement)
        return GridMovementComponent::MovementDirection::None;
        
    Position pos = movement->GetGridPosition();
    
    GridMovementComponent::MovementDirection directions[] = {
        GridMovementComponent::MovementDirection::Up,
        GridMovementComponent::MovementDirection::Down,
        GridMovementComponent::MovementDirection::Left,
        GridMovementComponent::MovementDirection::Right
    };

	int randomIndex = rand() % 4; // Randomly select a direction to start with
    
 
    int newX = pos.x;
    int newY = pos.y;
    
    switch (directions[randomIndex])
    {
    case GridMovementComponent::MovementDirection::Up:
        newY--; break;
    case GridMovementComponent::MovementDirection::Down:
        newY++; break;
    case GridMovementComponent::MovementDirection::Left:
        newX--; break;
    case GridMovementComponent::MovementDirection::Right:
        newX++; break;
    }
    
    if (movement->CanMoveToPosition(newX, newY))
        return directions[randomIndex];
    
    return GridMovementComponent::MovementDirection::None;
}

void EnemyIdleState::Enter(Enemy* enemy)
{
    m_IdleTime = 0.0f;
    
    // Visual indicator for idle state (could change animation/texture)
    if (auto owner = enemy->GetOwner())
    {
        if (auto render = owner->GetComponent<yev::RenderComponent>())
        {
            // Change appearance for idle state
            // render->SetTexture("EnemyIdle.png");
        }
    }
}

void EnemyIdleState::Update(Enemy* enemy, float deltaTime)
{
    m_IdleTime += deltaTime;
    
    if (m_IdleTime >= m_MaxIdleTime)
    {
        auto newState = HandleStateExpired(enemy);
        if (newState)
            enemy->ChangeState(std::move(newState));
    }
}

void EnemyIdleState::Exit(Enemy*)
{
    // Clean up idle state resources if needed
}

std::unique_ptr<EnemyState> EnemyIdleState::HandlePlayerSeen(Enemy*)
{
    return std::make_unique<EnemyChaseState>();
}

std::unique_ptr<EnemyState> EnemyIdleState::HandleStateExpired(Enemy*)
{
    return std::make_unique<EnemyPatrolState>();
}

void EnemyPatrolState::Enter(Enemy* enemy)
{
    m_DirectionTimer = 0.0f;
    
    if (auto map = enemy->GetMap())
    {
        m_CurrentDirection = FindValidDirection(enemy, map);
    }
    
    if (auto owner = enemy->GetOwner())
    {
        if (auto render = owner->GetComponent<yev::RenderComponent>())
        {
            // Change appearance for patrol state
            // render->SetTexture("EnemyPatrol.png");
        }
    }
}

void EnemyPatrolState::Update(Enemy* enemy, float deltaTime)
{
    auto movement = enemy->GetMovement();
    if (!movement)
        return;
    
    m_DirectionTimer += deltaTime;
    if (m_DirectionTimer >= m_DirectionChangeInterval)
    {
        m_DirectionTimer = 0.0f;
        
        // Change direction
        if (auto map = enemy->GetMap())
        {
            m_CurrentDirection = FindValidDirection(enemy, map);
        }
    }

    bool moved = false;
    switch (m_CurrentDirection)
    {
    case GridMovementComponent::MovementDirection::Up:
        moved = movement->MoveUp();
        break;
    case GridMovementComponent::MovementDirection::Down:
        moved = movement->MoveDown();
        break;
    case GridMovementComponent::MovementDirection::Left:
        moved = movement->MoveLeft();
        break;
    case GridMovementComponent::MovementDirection::Right:
        moved = movement->MoveRight();
        break;
    default:   
        if (auto map = enemy->GetMap())
        {
            m_CurrentDirection = FindValidDirection(enemy, map);
        }
        break;
    }
    
    if (!moved)
    {
        if (auto map = enemy->GetMap())
        {
            m_CurrentDirection = FindValidDirection(enemy, map);
        }
    }
}

void EnemyPatrolState::Exit(Enemy*)
{
    // Clean up patrol state resources if needed
}

std::unique_ptr<EnemyState> EnemyPatrolState::HandlePlayerSeen(Enemy*)
{
    return std::make_unique<EnemyChaseState>();
}

void EnemyChaseState::Enter(Enemy* enemy)
{
    m_PathUpdateTimer = 0.0f;
    
    if (auto owner = enemy->GetOwner())
    {
        if (auto render = owner->GetComponent<yev::RenderComponent>())
        {
            // Change appearance for chase state
            // render->SetTexture("EnemyChase.png");
        }
    }
    
    // For actual game: increase speed during chase
    if (auto movement = enemy->GetMovement())
    {
        movement->SetMovementSpeed(enemy->GetMoveSpeed() * 1.2f);
    }
}

void EnemyChaseState::Update(Enemy* enemy, float deltaTime)
{
    // In a complete implementation, this would use pathfinding to chase the player
    // For now, we'll just use a simple movement approach
    
    m_PathUpdateTimer += deltaTime;
    if (m_PathUpdateTimer >= m_PathUpdateInterval)
    {
        m_PathUpdateTimer = 0.0f;
        
        // Update chase path - simplified version
        // In a real implementation, this would calculate A* or similar pathfinding
        if (auto movement = enemy->GetMovement())
        {
            // Placeholder: just move in a valid direction
            if (auto map = enemy->GetMap())
            {
                auto direction = FindValidDirection(enemy, map);
                switch (direction)
                {
                case GridMovementComponent::MovementDirection::Up:
                    movement->MoveUp();
                    break;
                case GridMovementComponent::MovementDirection::Down:
                    movement->MoveDown();
                    break;
                case GridMovementComponent::MovementDirection::Left:
                    movement->MoveLeft();
                    break;
                case GridMovementComponent::MovementDirection::Right:
                    movement->MoveRight();
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void EnemyChaseState::Exit(Enemy* enemy)
{
    // Reset movement speed to normal
    if (auto movement = enemy->GetMovement())
    {
        movement->SetMovementSpeed(enemy->GetMoveSpeed());
    }
}

std::unique_ptr<EnemyState> EnemyChaseState::HandlePlayerLost(Enemy*)
{
    return std::make_unique<EnemyPatrolState>();
}

std::unique_ptr<EnemyState> EnemyChaseState::HandleDamaged(Enemy*)
{
    return std::make_unique<EnemyStunnedState>();
}

void EnemyStunnedState::Enter(Enemy* enemy)
{
    m_StunTimer = 0.0f;
    
    if (auto owner = enemy->GetOwner())
    {
        if (auto render = owner->GetComponent<yev::RenderComponent>())
        {
            // Change appearance for stunned state
            // render->SetTexture("EnemyStunned.png");
        }
    }
}

void EnemyStunnedState::Update(Enemy* enemy, float deltaTime)
{
    m_StunTimer += deltaTime;
    
    // Recover from stun after duration expires
    if (m_StunTimer >= m_StunDuration)
    {
        auto newState = HandleStateExpired(enemy);
        if (newState)
            enemy->ChangeState(std::move(newState));
    }
}

void EnemyStunnedState::Exit(Enemy*)
{
    // Clean up stunned state resources if needed
}

std::unique_ptr<EnemyState> EnemyStunnedState::HandleStateExpired(Enemy*)
{
    return std::make_unique<EnemyIdleState>();
}

void EnemyDeathState::Enter(Enemy* enemy)
{
    m_DeathTimer = 0.0f;
    m_AnimationComplete = false;
    
    if (auto owner = enemy->GetOwner())
    {
        if (auto render = owner->GetComponent<yev::RenderComponent>())
        {
            // Change appearance for death state
            // render->SetTexture("EnemyDeath.png");
        }
    }
}

void EnemyDeathState::Update(Enemy* enemy, float deltaTime)
{
    if (m_AnimationComplete)
        return;
    
    m_DeathTimer += deltaTime;
    
    if (m_DeathTimer >= m_DeathAnimationDuration)
    {
        m_AnimationComplete = true;
        
        // In a complete implementation, the enemy would be removed from the game
        // or recycled into an object pool
        if (auto owner = enemy->GetOwner())
        {
            // Mark for destruction or disable
            // owner->SetActive(false);
        }
    }
}

void EnemyDeathState::Exit(Enemy*)
{
    // Clean up death state resources if needed
}