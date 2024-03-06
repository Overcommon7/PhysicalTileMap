#pragma once
#include "IWindow.h"
#include "Camera/Camera.h"

class ITextureWindow : public IWindow  
{    
public:
    virtual void Initialize() override {}
    virtual void Terminate() override;

    virtual void Update() override;

    virtual void BeginDraw();
    virtual void EndDraw();
    virtual void ImGuiDraw() override;

    void DrawDebugMenuItems() override;


    void SetClearColor(Color color) { mCamera.SetClearColor(color); }
    void SetTintColor(Color color) { mCamera.SetTintColor(color); }

    Render::Camera& GetCamera() { return mCamera; }
    Vector2Int GetMousePosition() const;
    Vector2Int GetTexturePoint(Vector2Int point) const ;
    bool IsInsideTexture(Vector2Int point) const;
protected:
    ITextureWindow(const string& title, Vector2Int cameraResolution);

    virtual void RaylibDraw() = 0;
    Render::Camera mCamera;
    Vector2Int mLocalMousePosition;
    Vector2Int mTextureSize;
    Vector2Int mTexturePosition;
private:
    void DrawTextureToWindow();
};

#define ITextureWindowConstructor(className) className(const string& windowTitle, Vector2Int cameraResolution) : ITextureWindow(windowTitle, cameraResolution)
#define ITextureWindowConstructorHeader(className) className(const string& windowTitle, Vector2Int cameraResolution)

