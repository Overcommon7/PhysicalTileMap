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
        Rigidbody* rigidbody = nullptr;
        Sprite* sprite = nullptr;

        void Initialize();
        void Terminate();
    };
public:
    static void Update(Render::Camera& camera, Values& values, const Player& player);
    static void ImGuiDraw(Values* values);
private:
public:
    CameraMovement() = delete;
    CameraMovement(const CameraMovement& c) = delete;
    CameraMovement(const CameraMovement&& c) = delete;
    CameraMovement& operator=(const CameraMovement& c) = delete;
    CameraMovement& operator=(const CameraMovement&& c) = delete;

};

