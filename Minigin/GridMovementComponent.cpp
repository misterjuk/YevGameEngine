#include "GridMovementComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Time.h"
#include <iostream>
#include "Map.h"


GridMovementComponent::GridMovementComponent(yev::GameObject* ownerObjectPtr, Map* map)
    : yev::Component(ownerObjectPtr), m_Map(map)
{
    m_Transform = ownerObjectPtr->GetComponent<yev::TransformComponent>();

    // Initialize to player spawn position if available
    if (m_Map)
    {
        m_GridPosition = m_Map->GetPlayerSpawnPosition();
        UpdateVisualPosition();
    }
}

void GridMovementComponent::UpdatePosition()
{
    if (!m_Transform || !m_Map) return;

   
    glm::vec3 currentPos = m_Transform->GetLocalPosition();

    int tileSize = m_Map->GetTileSize();
    int currentGridX = static_cast<int>(std::round(currentPos.x / tileSize));
    int currentGridY = static_cast<int>(std::round(currentPos.y / tileSize));

	SetGridPosition(currentGridX, currentGridY);

    m_TargetPosition = glm::vec3(
        currentGridX * tileSize,
        currentGridY * tileSize,
        0.0f
    );

    glm::vec3 directionToTargetPos = m_TargetPosition - currentPos;


    std::cout << "Current grid x,y: " << currentGridX << ":"<< currentPos.x << "," << currentGridY <<  ":" << currentPos.y << std::endl;

    float deltaTime = yev::Time::GetInstance().GetDeltaTime();
    float moveAmount = m_MovementSpeed * deltaTime;

    
    if (m_IsWalkingOnXAxis && (m_Direction == MovementDirection::Left || m_Direction == MovementDirection::Right))
    {
        switch (m_Direction) {
        case GridMovementComponent::MovementDirection::Left:

            m_Transform->Translate(glm::vec3(-1, 0, 0) * moveAmount);
            m_IsWalkingOnXAxis = true;
            break;
        case GridMovementComponent::MovementDirection::Right:

            m_Transform->Translate(glm::vec3(1, 0, 0) * moveAmount);
            m_IsWalkingOnXAxis = true;
            break;
        }
	
    }
	else if (!m_IsWalkingOnXAxis && (m_Direction == MovementDirection::Up || m_Direction == MovementDirection::Down))
    {
        switch (m_Direction)
        {
        case GridMovementComponent::MovementDirection::Up:

            m_Transform->Translate(glm::vec3(0, -1, 0) * moveAmount);
            m_IsWalkingOnXAxis = false;
            break;
        case GridMovementComponent::MovementDirection::Down:

            m_Transform->Translate(glm::vec3(0, 1, 0) * moveAmount);
            m_IsWalkingOnXAxis = false;
            break;
        }
    }
    else
    {
        //check if close enough to switch ditrections else
		//move towards the middle of the tile

        float distance = glm::length(directionToTargetPos);
        if (distance < 1.0f)
        {
            switch (m_Direction)
            {
            case GridMovementComponent::MovementDirection::Up:

                m_Transform->Translate(glm::vec3(0, -1, 0) * moveAmount);
                m_IsWalkingOnXAxis = false;
                break;
            case GridMovementComponent::MovementDirection::Down:

                m_Transform->Translate(glm::vec3(0, 1, 0) * moveAmount);
                m_IsWalkingOnXAxis = false;
                break;

            case GridMovementComponent::MovementDirection::Left:

                m_Transform->Translate(glm::vec3(-1, 0, 0) * moveAmount);
                m_IsWalkingOnXAxis = true;
                break;
            case GridMovementComponent::MovementDirection::Right:

                m_Transform->Translate(glm::vec3(1, 0, 0) * moveAmount);
                m_IsWalkingOnXAxis = true;
                break;
            }
        }
        else 
        {
            m_Transform->Translate(directionToTargetPos/ distance * moveAmount);
        }
    }

    
}

void GridMovementComponent::SetGridPosition(int x, int y)
{
    if (m_Map->IsValidPosition(x, y))
    {
        m_GridPosition = { x, y };
        UpdateVisualPosition();
    }
}

bool GridMovementComponent::MoveUp()
{
    m_Direction = MovementDirection::Up;
    UpdatePosition();
    UpdateVisualPosition();
    DigAtCurrentPosition();
    return true;
}

bool GridMovementComponent::MoveDown()
{
    m_Direction = MovementDirection::Down;
    UpdatePosition();
    UpdateVisualPosition();
    DigAtCurrentPosition();
    return true;
}

bool GridMovementComponent::MoveLeft()
{
    m_Direction = MovementDirection::Left;
    UpdatePosition();
    UpdateVisualPosition();
    DigAtCurrentPosition();
    return true;
}

bool GridMovementComponent::MoveRight()
{
    m_Direction = MovementDirection::Right;
    UpdatePosition();
    UpdateVisualPosition();
    DigAtCurrentPosition();
    return true;
}

void GridMovementComponent::DigAtCurrentPosition()
{
    if (m_Map)
    {

        m_Map->DigTunnel(m_GridPosition.x, m_GridPosition.y);
    }
}

bool GridMovementComponent::CanMoveToPosition(int x, int y) const
{
    if (!m_Map || !m_Map->IsValidPosition(x, y))
        return false;

    TileType tileType = m_Map->GetTileAt(x, y);

    // Can move to empty spaces, earth tiles, and spawns
    return tileType != TileType::Rock;
}

void GridMovementComponent::UpdateVisualPosition()
{
    if (!m_Transform || !m_Map) return;

    //// Calculate pixel position from grid position
    //int tileSize = m_Map->GetTileSize();
    //float x = static_cast<float>(m_GridPosition.x * tileSize);
    //float y = static_cast<float>(m_GridPosition.y * tileSize);

    //// Store as target position and set the character to moving state
    //m_TargetPosition = { x, y, m_Transform->GetLocalPosition().z };
    //m_IsMoving = true;
}



GridMovementComponent::MovementDirection GridMovementComponent::GetDirection() const
{
    return m_Direction;
}

void GridMovementComponent::SetMovementSpeed(float speed)
{
    m_MovementSpeed = speed;
}

float GridMovementComponent::GetMovementSpeed() const
{
    return m_MovementSpeed;
}