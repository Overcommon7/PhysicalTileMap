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
	{
		if (window->IsClosable() && !window->IsOpen())
			continue;

		if (sIsPopupOpen && sOpenPopup != window.get())
			continue;

		window->Update();
	}
		
}

void ILayer::RaylibDraw()
{
	for (const auto& window : mWindows)
	{
		if (window->IsClosable() && !window->IsOpen())
			continue;

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
		if (window->IsClosable() && !window->IsOpen())
			continue;

		window->ImGuiDrawBegin();
		window->ImGuiDraw();
		window->ImGuiDrawEnd();
	}

}

void ILayer::SetCurrentPopup(IWindow* currentPopup)
{
	if (currentPopup == nullptr)
	{
		sOpenPopup = nullptr;
		sIsPopupOpen = false;
	}
	else
	{
		sOpenPopup = currentPopup;
		sIsPopupOpen = true;
	}


}
