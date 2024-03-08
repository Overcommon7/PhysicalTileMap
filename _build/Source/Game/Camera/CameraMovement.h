#pragma once

namespace Render
{
    class Camera;
}

class Player;
class Rigidbody;
class Sprite;

class CameraMovement
{
public:
    struct Values
    {
        float minSpeed = 30.f;
        float minDistance = 10.f;
        float distanceEffector = 0.8f;
        Vector2 lastPosition{};
        Vector2 positionDelta{};
    };
public:
    static void FixedUpdate(Render::Camera& camera, Values& values, const Player& player);
    static void ImGuiDraw(Render::Camera& camera, Values* values);
private:
public:
    CameraMovement() = delete;
    CameraMovement(const CameraMovement& c) = delete;
    CameraMovement(const CameraMovement&& c) = delete;
    CameraMovement& operator=(const CameraMovement& c) = delete;
    CameraMovement& operator=(const CameraMovement&& c) = delete;

};

