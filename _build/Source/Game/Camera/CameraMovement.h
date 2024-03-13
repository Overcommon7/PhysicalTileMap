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
    enum class Type
    {
        Fixed = 0,
        PlayerPush,
        Smooth,
        UpdateOnLand,
        
    };

    enum class HorizantalMoveType
    {
        Bounds = 0,
        Fixed,
        Smooth
    };

    struct Values
    {
        Type mMovementType = Type::Smooth;
        Vector2 mLastPosition{};
        Vector2 mPositionDelta{};
        Vector2 mRelativePosition{};

        struct Push
        {
            float xMoveThreshold = 0.75f;
            float yMoveThreshold = 0.75f;
            Vector2 min{}, max{};
        };

        struct OnLand
        {
            float evenOutSpeed = 700.f;
            float evenOutTarget{};
            bool isEveningOut = false;
            HorizantalMoveType moveType = HorizantalMoveType::Fixed;
        };

        struct Smooth
        {
            float distanceEffector = 0.8f;
            float minEffectDistance = 24.f;
            float minSpeed = 72.f;
        };
        
        Push mPush;
        OnLand mOnLand;
        Smooth mSmooth;
    };
public:
    static void FixedUpdate(Render::Camera& camera, Values& values, const Player& player, const float timeStep);
    static void ImGuiDraw(Render::Camera& camera, Values* values);
private:
    static void Fixed(Render::Camera& camera, Values& values, const Player& player);
    static void Smooth(Render::Camera& camera, Values& values, const Player& player, float timeStep);
    static void UpdateOnLand(Render::Camera& camera, Values& values, const Player& player, const float timeStep);
    static void PlayerPush(Render::Camera& camera, Values& values, const Player& player, bool doX = true, bool doY = true);
public:
    CameraMovement() = delete;
    CameraMovement(const CameraMovement& c) = delete;
    CameraMovement(const CameraMovement&& c) = delete;
    CameraMovement& operator=(const CameraMovement& c) = delete;
    CameraMovement& operator=(const CameraMovement&& c) = delete;

};

