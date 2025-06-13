#include "GridMovementComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Time.h"
#include <iostream>
#include "Map.h"
#include "ScoreComponent.h"


GridMovementComponent::GridMovementComponent(yev::GameObject* ownerObjectPtr, Map* map, bool isPlayer)
    : yev::Component(ownerObjectPtr), m_Map(map), m_IsPlayer{isPlayer}
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

	m_GridPosition = { currentGridX, currentGridY };

    m_TargetPosition = glm::vec3(
        currentGridX * tileSize,
        currentGridY * tileSize,
        0.0f
    );

    glm::vec3 directionToTargetPos = m_TargetPosition - currentPos;

    if(m_IsPlayer)
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
        if (distance < 3.0f)
        {
            m_Transform->SetLocalPosition(m_TargetPosition);
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

        int tileSize = m_Map->GetTileSize();
        float calculatedX = static_cast<float>(m_GridPosition.x * tileSize);
        float calculatedY = static_cast<float>(m_GridPosition.y * tileSize);

        m_Transform->SetLocalPosition(calculatedX, calculatedY, m_Transform->GetLocalPosition().z);
    }
}

bool GridMovementComponent::MoveUp()
{
    if (CanMoveToPosition(m_GridPosition.x, m_GridPosition.y - 1) == false)
    {
        return false;
    }
    m_Direction = MovementDirection::Up;
    UpdatePosition();
    UpdateVisualPosition();
    DigAtCurrentPosition();
    return true;
}

bool GridMovementComponent::MoveDown()
{

    if (CanMoveToPosition(m_GridPosition.x, m_GridPosition.y + 1) == false)
    {
        return false;
    }
    m_Direction = MovementDirection::Down;
    UpdatePosition();
    UpdateVisualPosition();
    DigAtCurrentPosition();
    return true;
}

bool GridMovementComponent::MoveLeft()
{
    if (CanMoveToPosition(m_GridPosition.x - 1, m_GridPosition.y) == false)
    {
        return false;
    } 
    m_Direction = MovementDirection::Left;
    UpdatePosition();
    UpdateVisualPosition();

    DigAtCurrentPosition();
    return true;
}

bool GridMovementComponent::MoveRight()
{
    if (CanMoveToPosition(m_GridPosition.x + 1, m_GridPosition.y) == false)
    {
        return false;
    }
		
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
        if (m_IsPlayer)
        {
            bool tileWasEarth = m_Map->GetTileAt(m_GridPosition.x, m_GridPosition.y) == TileType::Earth;
            m_Map->DigTunnel(m_GridPosition.x, m_GridPosition.y);

            if (tileWasEarth && GetOwner())
            {
                if (m_Owner->HasComponent<ScoreComponent>())
                {
                    m_Owner->GetComponent<ScoreComponent>()->AddScore(10);
                }               
            }
        }
    }
}

bool GridMovementComponent::CanMoveToPosition(int x, int y) const
{
    if (!m_Map || !m_Map->IsValidPosition(x, y))
        return false;

    TileType tileType = m_Map->GetTileAt(x, y);

    // Can move to empty spaces, earth tiles, and spawns

    if(m_IsPlayer)
    {
        if (tileType == TileType::Earth || tileType == TileType::Empty || tileType == TileType::PlayerSpawn || tileType == TileType::EnemySpawn)
            return true;
    }
    else
    {
        if (tileType == TileType::Empty || tileType == TileType::PlayerSpawn || tileType == TileType::EnemySpawn)
            return true;
	}
    return false;
}

void GridMovementComponent::UpdateVisualPosition()
{
 
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