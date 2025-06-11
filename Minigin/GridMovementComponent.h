#pragma once
#include "Component.h"
#include "Position.h"
#include <memory>
#include <glm.hpp>
// Forward declarations
class Map;

namespace yev 
{
    class TransformComponent;
}

class GridMovementComponent final : public yev::Component
{
public:
    enum class MovementDirection
    {
        Up,
        Down,
        Left,
        Right,
        None
    };

    GridMovementComponent(yev::GameObject* ownerObjectPtr, Map* map);
    ~GridMovementComponent() = default;

    void UpdatePosition();

    void SetGridPosition(int x, int y);
    Position GetGridPosition() const { return m_GridPosition; }

    bool MoveUp();
    bool MoveDown();
    bool MoveLeft();
    bool MoveRight();

    void DigAtCurrentPosition();
    bool CanMoveToPosition(int x, int y) const;

    MovementDirection GetDirection() const;

    void SetMovementSpeed(float speed);
    float GetMovementSpeed() const;

private:
    void UpdateVisualPosition();

    Position m_GridPosition{ 0, 0 };
    Map* m_Map{ nullptr };
    yev::TransformComponent* m_Transform{ nullptr };

    MovementDirection m_Direction{ MovementDirection::None };

    float m_MovementSpeed{ 150.0f };

    glm::vec3 m_TargetPosition{ 0, 0, 0 };
    bool m_CanRotate{ true };
    bool m_IsWalkingOnXAxis{ true };
};