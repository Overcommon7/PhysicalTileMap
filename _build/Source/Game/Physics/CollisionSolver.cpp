#include "pch.h"
#include "CollisionSolver.h"

#include "Game/Physics/Rigidbody.h"
#include "Game/Sprites/Sprite.h"
#include "Editor/Project/Project.h"


void CollisionSolver::RigidbodyRigidbodyCollision(Rigidbody* rigidbody, Rigidbody* other)
{
    Rectangle overlap(0, 0, -1, -1);
    if (!TryGetRectangleOverlap(rigidbody->mSprite->Collider(), other->mSprite->Collider(), overlap))
        return;

    rigidbody->mCollisions.emplace_back(other, overlap);
}

void CollisionSolver::RigidbodyTilemapCollision(Rigidbody* rigidbody, Project* project, Conversion screenToGrid, Conversion gridToScreen)
{
    RectangleCorners corners(GetRectangleCorners(rigidbody->mSprite->Collider(), screenToGrid));

    if (project->GetTile(corners.bottom.left).has_value())
        corners.bottom.left = SolveBottomLeftCorner(rigidbody, corners.bottom.left, project);

    if (project->GetTile(corners.bottom.right).has_value())
        corners.bottom.right = SolveBottomRightCorner(rigidbody, corners.bottom.right, project);

    if (project->GetTile(corners.top.left).has_value())
        corners.bottom.left = SolveTopLeftCorner(rigidbody, corners.top.left, project);

    if (project->GetTile(corners.top.right).has_value())
        corners.top.right = SolveTopRightCorner(rigidbody, corners.top.right, project);
}

bool CollisionSolver::TryGetRectangleOverlap(Rectangle a, Rectangle b, Rectangle& overlap)
{
    float right_a = a.x + a.width;
    float right_b = b.x + b.width;
    float bottom_a = a.y + a.height;
    float bottom_b = b.y + b.height;

    float left = std::max(a.x, b.x);
    float top = std::max(a.y, b.y);
    float right = std::min(right_a, right_b);
    float bottom = std::min(bottom_a, bottom_b);

    if ((left > right) || (top > bottom))
        return false;

    overlap.x = left;
    overlap.y = top;
    overlap.width = right - left;
    overlap.height = bottom - top;

    return true;
}

RectangleCorners CollisionSolver::GetRectangleCorners(Rectangle collider, Conversion screenToGrid)
{
    RectangleCorners corners;

    corners.top.left = screenToGrid({ collider.x, collider.y });
    corners.top.right = screenToGrid({ collider.x + collider.width, collider.y });

    corners.bottom.left = screenToGrid({ collider.x, collider.y });
    corners.bottom.right = screenToGrid({ collider.x + collider.width, collider.y + collider.height });

    return corners;
}

Vector2Int CollisionSolver::SolveTopRightCorner(Rigidbody* rigidbody, Vector2Int corner, Project* project)
{
    
}

Vector2Int CollisionSolver::SolveTopLeftCorner(Rigidbody* rigidbody, Vector2Int corner, Project* project)
{
    
}

Vector2Int CollisionSolver::SolveBottomRightCorner(Rigidbody* rigidbody, Vector2Int corner, Project* project)
{
    
}

Vector2Int CollisionSolver::SolveBottomLeftCorner(Rigidbody* rigidbody, Vector2Int corner, Project* project)
{
   
}
