#include "Player.h"
#include "GameObject.h"
#include "Time.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "GameEvents.h" 
#include "GridMovementComponent.h"
#include "Map.h"
#include <algorithm>
#include "InputManager.h"
#include "PlayerCommands.h"

Player::Player(yev::GameObject* ownerObjectPtr, Map* map)
    : yev::Component(ownerObjectPtr)
    , m_Map(map)
{
    // Get required components
    if (ownerObjectPtr)
    {
        m_Transform = ownerObjectPtr->GetComponent<yev::TransformComponent>();
        m_RenderComponent = ownerObjectPtr->GetComponent<yev::RenderComponent>();
        
        // Add grid movement component if missing
        if (!ownerObjectPtr->HasComponent<GridMovementComponent>())
        {
            ownerObjectPtr->AddComponent<GridMovementComponent>(ownerObjectPtr, map, true);
        }
    }
    
    InitializePlayer();
    BindPlayerInput();
}

void Player::Update()
{
    // Skip update if inactive or no state
    if (!m_CurrentState || !IsAlive())
        return;
    
    float deltaTime = yev::Time::GetInstance().GetDeltaTime();
    
    // Update the current state
    m_CurrentState->Update(this, deltaTime);
}

void Player::Render() const
{
    // Player-specific rendering can be added here if needed
    // The GameObject's RenderComponent will handle basic rendering
}

void Player::ChangeState(std::unique_ptr<PlayerState> newState)
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

void Player::BindPlayerInput()
{
    yev::InputManager& inputManager = yev::InputManager::GetInstance();

    inputManager.BindKeyboardCommand(SDLK_a, yev::InputState::Held, std::make_unique<PlayerMoveLeftCommand>(m_Owner));
    inputManager.BindKeyboardCommand(SDLK_d, yev::InputState::Held, std::make_unique<PlayerMoveRightCommand>(m_Owner));
    inputManager.BindKeyboardCommand(SDLK_w, yev::InputState::Held, std::make_unique<PlayerMoveUpCommand>(m_Owner));
    inputManager.BindKeyboardCommand(SDLK_s, yev::InputState::Held, std::make_unique<PlayerMoveDownCommand>(m_Owner));
}

void Player::HandleMovementInput(GridMovementComponent::MovementDirection direction)
{
    if (!IsAlive() || !m_CurrentState)
        return;
    
    auto newState = m_CurrentState->HandleMovementInput(this, direction);
    if (newState)
        ChangeState(std::move(newState));
}

void Player::HandleDigInput()
{
    if (!IsAlive() || !m_CurrentState)
        return;
    
    auto newState = m_CurrentState->HandleDigInput(this);
    if (newState)
        ChangeState(std::move(newState));
}

void Player::HandleAttackInput()
{
    if (!IsAlive() || !m_CurrentState)
        return;
    
    auto newState = m_CurrentState->HandleAttackInput(this);
    if (newState)
        ChangeState(std::move(newState));
}

void Player::TakeDamage(int damage)
{
    m_Health = std::max(0, m_Health - damage);
    
    // Notify observers that player was damaged
    NotifyObservers(GameEvents::PlayerDamaged, GetOwner());
    
    if (m_Health <= 0 && !m_IsDying)
    {
        Kill();
    }
}

void Player::Kill()
{
    m_IsDying = true;
    
    // Notify observers that player died
    NotifyObservers(GameEvents::PlayerDied, GetOwner());
    
    // Change to death state
    ChangeState(std::make_unique<PlayerDyingState>());
}

void Player::AddScore(int points)
{
    m_Score += points;
    
    // Notify observers that player scored
    NotifyObservers(GameEvents::PlayerScored, GetOwner());
}

GridMovementComponent* Player::GetMovement() const
{
    return GetOwner() ? GetOwner()->GetComponent<GridMovementComponent>() : nullptr;
}

void Player::InitializePlayer()
{
    // Set up the initial state (Idle)
    ChangeState(std::make_unique<PlayerIdleState>());
    
    // Apply move speed to the movement component
    if (auto movement = GetMovement())
    {
        movement->SetMovementSpeed(m_MoveSpeed);
    }
}