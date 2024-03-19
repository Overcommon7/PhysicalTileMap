#pragma once
#include "ImGuiWindow/IWindow.h"


class ILayer
{
public:
	ILayer() = default;
	virtual ~ILayer() = default;

	virtual void Initialize();
	virtual void Shutdown();
	virtual void Update();
	virtual void RaylibDraw();
	virtual void ImGuiDraw();

	static void SetCurrentPopup(IWindow* currentPopup);
	static bool IsPopupOpen() { return sOpenPopup; }
protected:
	vector<unique_ptr<IWindow>> mWindows;

	static inline bool sIsPopupOpen = false;
	static inline IWindow* sOpenPopup = nullptr;
};

