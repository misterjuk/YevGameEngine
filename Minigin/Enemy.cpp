#include "Enemy.h"
#include "GameObject.h"
#include "Time.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "GameEvents.h"
#include "EnemyState.h" 
#include "GridMovementComponent.h"
#include "Map.h"
#include <algorithm>

Enemy::Enemy(yev::GameObject* ownerObjectPtr, Map* map, EnemyType type)
    : yev::Component(ownerObjectPtr)
    , m_Map(map)
    , m_Type(type)
{
    // Get required components
    if (ownerObjectPtr)
    {
        m_Transform = ownerObjectPtr->GetComponent<yev::TransformComponent>();
        m_RenderComponent = ownerObjectPtr->GetComponent<yev::RenderComponent>();
        
        // Add grid movement component if missing
        if (!ownerObjectPtr->HasComponent<GridMovementComponent>())
        {
            ownerObjectPtr->AddComponent<GridMovementComponent>(ownerObjectPtr, map);
        }
    }
    
    InitializeAI();
}

void Enemy::Update()
{
    // Skip update if inactive or no state
    if (!m_CurrentState || !IsAlive())
        return;
        
    float deltaTime = yev::Time::GetInstance().GetDeltaTime();
    // Update player detection
    m_PlayerDetectionTimer += deltaTime;
    if (m_PlayerDetectionTimer >= m_PlayerDetectionInterval)
    {
        m_PlayerDetectionTimer = 0;
        
        // Here would be code to detect player - simplified for now
        // Check if player is within vision range and not blocked by walls
        bool playerWasDetected = m_PlayerDetected;
        m_PlayerDetected = false; // Actual detection code would go here
        
        // Handle state changes based on player detection
        if (m_PlayerDetected && !playerWasDetected)
        {
            auto newState = m_CurrentState->HandlePlayerSeen(this);
            if (newState)
                ChangeState(std::move(newState));
        }
        else if (!m_PlayerDetected && playerWasDetected)
        {
            auto newState = m_CurrentState->HandlePlayerLost(this);
            if (newState)
                ChangeState(std::move(newState));
        }
    }
    
    // Update the current state
    m_CurrentState->Update(this, deltaTime);
}

void Enemy::Render() const
{
    // Enemy-specific rendering can be added here if needed
    // The GameObject's RenderComponent will handle basic rendering
}

void Enemy::ChangeState(std::unique_ptr<EnemyState> newState)
{
    // Exit the current state
    if (m_CurrentState)
        m_CurrentState->Exit(this);
        
    // Change to the new state
    m_CurrentState = std::move(newState);
    
    // Enter the new state
    if (m_CurrentState)
        m_CurrentState->Enter(this);
}

void Enemy::TakeDamage(int damage)
{
    m_Health = std::max(0, m_Health - damage);
    
    if (m_Health <= 0)
    {
        Kill();
    }
    else
    {
        // Notify observers that enemy was damaged
        NotifyObservers(GameEvents::EnemyKilled, GetOwner());
        
        // Handle state change for being damaged
        if (m_CurrentState)
        {
            auto newState = m_CurrentState->HandleDamaged(this);
            if (newState)
                ChangeState(std::move(newState));
        }
    }
}

void Enemy::Kill()
{
    m_Health = 0;
    
    // Notify observers that enemy was killed
    NotifyObservers(GameEvents::EnemyKilled, GetOwner());
    
    // Change to death state
    ChangeState(std::make_unique<EnemyDeathState>());
}

void Enemy::Stun()
{
    // Change to stunned state
    ChangeState(std::make_unique<EnemyStunnedState>());
}

GridMovementComponent* Enemy::GetMovement() const
{
    return GetOwner() ? GetOwner()->GetComponent<GridMovementComponent>() : nullptr;
}

void Enemy::InitializeAI()
{
    // Set up the initial state (Idle)
    ChangeState(std::make_unique<EnemyIdleState>());
    
    // Configure enemy type-specific properties
    if (m_Type == EnemyType::Fygar)
    {
        m_Health = 3;        // Fygar is tougher
        m_MoveSpeed = 80.0f; // But slower
    }
    else // Pooka
    {
        m_Health = 2;
        m_MoveSpeed = 120.0f;
    }
    
    // Apply move speed to the movement component
    if (auto movement = GetMovement())
    {
        movement->SetMovementSpeed(m_MoveSpeed);
    }
}