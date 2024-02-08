#pragma once
#include "IWindow.h"
#include "Camera/Camera.h"

class ITextureWindow : public IWindow  
{    
public:
    virtual void Initialize() override {}
    virtual void Terminate() override;

    virtual void BeginDraw();
    virtual void EndDraw();
    virtual void ImGuiDraw() override;

    void DrawDebugMenuItems() override;


    void SetClearColor(Color color) { camera.SetClearColor(color); }
    void SetTintColor(Color color) { camera.SetTintColor(color); }

    Render::Camera& GetCamera() { return camera; }
    Vector2Int GetMousePosition();
    Vector2Int GetTexturePoint(Vector2Int point);
    bool IsInsideTexture(Vector2Int point);
protected:
    ITextureWindow(const string& title, Vector2Int cameraResolution);

    virtual void RaylibDraw() = 0;
    Render::Camera camera;
    Vector2Int localMousePosition;
    Vector2Int textureSize;
    Vector2Int texturePosition;
private:
    void DrawTextureToWindow();
};

#define ITextureWindowConstructor(className) className(const string& windowTitle, Vector2Int cameraResolution) : ITextureWindow(windowTitle, cameraResolution)


