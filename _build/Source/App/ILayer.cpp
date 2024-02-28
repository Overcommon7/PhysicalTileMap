#include "pch.h"
#include "ILayer.h"

#include "ImGuiWindow/ITextureWindow.h"

void ILayer::Initialize()
{
	for (const auto& window : mWindows)
		window->Initialize();
}

void ILayer::Shutdown()
{
	for (const auto& window : mWindows)
		window->Terminate();
}

void ILayer::Update()
{
	for (auto& window : mWindows)
		window->Update();
}

void ILayer::RaylibDraw()
{
	for (const auto& window : mWindows)
	{
		ITextureWindow* textureWindow = dynamic_cast<ITextureWindow*>(window.get());
		if (!textureWindow)	continue;

		textureWindow->BeginDraw();
		textureWindow->EndDraw();
	}

}

void ILayer::ImGuiDraw()
{
	for (const auto& window : mWindows)
	{
		window->ImGuiDrawBegin();
		window->ImGuiDraw();
		window->ImGuiDrawEnd();
	}
}
