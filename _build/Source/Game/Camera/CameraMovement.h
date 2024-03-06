#pragma once

namespace Render
{
    class Camera;
}

class Player;

class CameraMovement
{
public:
    struct Values
    {

    };
public:
    static void Update(Render::Camera& camera, Values& values, const Player& player);
private:
public:
    CameraMovement() = delete;
    CameraMovement(const CameraMovement& c) = delete;
    CameraMovement(const CameraMovement&& c) = delete;
    CameraMovement& operator=(const CameraMovement& c) = delete;
    CameraMovement& operator=(const CameraMovement&& c) = delete;

};

