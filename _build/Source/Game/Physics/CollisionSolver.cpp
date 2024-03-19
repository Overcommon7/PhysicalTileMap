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
    ++sCollisionsSolved;
    ++sCollisionsSolvedThisSecond;
}

void CollisionSolver::RigidbodyTilemapCollision(Rigidbody* rigidbody, Project* project, Conversion screenToGrid, Conversion gridToScreen)
{
    bool stopX = false;
    bool stopY = false;
   
    auto c(rigidbody->mSprite->Collider());
    Vector2Int offset = project->GetTileSize() - Vector2Int(c.width, c.height);

    {  
        {
            auto collider = rigidbody->mSprite->Collider();
            Vector2Int corner = screenToGrid({ collider.x + 1, collider.y + collider.height });
            SolveSide(SolveBottomCollision, rigidbody, project, corner, offset, gridToScreen, stopX, stopY);
        }

        {
            auto collider = rigidbody->mSprite->Collider();
            Vector2Int corner = screenToGrid({ collider.x + collider.width - 1, collider.y + collider.height });
            SolveSide(SolveBottomCollision, rigidbody, project, corner, offset, gridToScreen, stopX, stopY);
        } 
    }   

    {
        auto collider = rigidbody->mSprite->Collider();
        Vector2Int corner = screenToGrid({ collider.x + (collider.width * 0.5f), collider.y });
        SolveSide(SolveTopCollision, rigidbody, project, corner, offset, gridToScreen, stopX, stopY); 
    }

    {
        auto collider = rigidbody->mSprite->Collider();
        Vector2Int corner = screenToGrid({ collider.x, collider.y + (collider.height * 0.5f) });
        SolveSide(SolveLeftCollision, rigidbody, project, corner, offset, gridToScreen, stopX, stopY);
    }

    {
        auto collider = rigidbody->mSprite->Collider();
        Vector2Int corner = screenToGrid({ collider.x + collider.width, collider.y + (collider.height * 0.5f) });
        SolveSide(SolveRightCollision, rigidbody, project, corner, offset, gridToScreen, stopX, stopY);
    }

    if (stopX)
        rigidbody->mVelocity.x = 0;
    if (stopY)
        rigidbody->mVelocity.y = 0;

    ++sCollisionsSolved;
    ++sCollisionsSolvedThisSecond;
}

void CollisionSolver::ImGuiDraw()
{
    if (ImGui::CollapsingHeader("Collision Resolver"))
    {
        ImGui::Text("Collisions Solved %zu", sCollisionsSolved);
        ImGui::Text("Collisions Solved This Second %i", sCollisionsSolvedLastSecond);
    }

    timer += Time::DeltaTime();
    if (timer < 1.f) return;

    sCollisionsSolvedLastSecond = sCollisionsSolvedThisSecond;
    sCollisionsSolvedThisSecond = 0;
    timer = 0;
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

void CollisionSolver::SolveTopCollision(Rigidbody* rigidbody, Vector2Int corner, Vector2Int offset, Conversion gridToScreen, Project* project, bool& stopX, bool& stopY)
{
    float position = rigidbody->mSprite->GetPosition().y;
  
    if (rigidbody->mVelocity.y < 0)
    {
        rigidbody->mCollidedTiles.at(CollisionEdge::Top).push_back(project->GetTile(corner).value());
        ++corner.y;
        position = gridToScreen(corner).y;
        stopY = true;
        
    }
    rigidbody->mSprite->SetPositionY(position);
}

void CollisionSolver::SolveBottomCollision(Rigidbody* rigidbody, Vector2Int corner, Vector2Int offset, Conversion gridToScreen, Project* project, bool& stopX, bool& stopY)
{
    float position = rigidbody->mSprite->GetPosition().y;

   
    if (rigidbody->mVelocity.y > 0)
    {
        rigidbody->mCollidedTiles.at(CollisionEdge::Bottom).push_back(project->GetTile(corner).value());
        --corner.y;        
        stopY = true;
        position = gridToScreen(corner).y + offset.y;
        rigidbody->mIsGrounded = true;
    }

    rigidbody->mSprite->SetPositionY(position);
}

void CollisionSolver::SolveLeftCollision(Rigidbody* rigidbody, Vector2Int corner, Vector2Int offset, Conversion gridToScreen, Project* project, bool& stopX, bool& stopY)
{
    float position = rigidbody->mSprite->GetPosition().x;

    if (rigidbody->mVelocity.x < 0)
    {
        rigidbody->mCollidedTiles.at(CollisionEdge::Left).push_back(project->GetTile(corner).value());
        ++corner.x;
        position = gridToScreen(corner).x;
        stopX = true;        
    }

    rigidbody->mSprite->SetPositionX(position);
}

void CollisionSolver::SolveRightCollision(Rigidbody* rigidbody, Vector2Int corner, Vector2Int offset, Conversion gridToScreen, Project* project, bool& stopX, bool& stopY)
{   
    float position = rigidbody->mSprite->GetPosition().x;
    
    if (rigidbody->mVelocity.x > 0)
    {
        rigidbody->mCollidedTiles.at(CollisionEdge::Right).push_back(project->GetTile(corner).value());
        --corner.x;
        position = gridToScreen(corner).x + offset.x;
        stopX = true;        
    }

    rigidbody->mSprite->SetPositionX(position);
}

void CollisionSolver::SolveSide(const std::function<void(Rigidbody*, Vector2Int, Vector2Int, Conversion, Project*, bool&, bool&)> solver,
    Rigidbody* rigidbody, Project* project, Vector2Int corner, Vector2Int offset, Conversion gridToScreen, 
    bool& stopX, bool& stopY)
{
    if (!project->GetTile(corner).has_value())
        return;

    solver(rigidbody, corner, offset, gridToScreen, project, stopX, stopY);
}

