#pragma once
#include "ImGuiWindow/IWindow.h"


class ILayer
{
public:
	ILayer() = default;
	virtual ~ILayer();

	virtual void Update();
	virtual void RaylibDraw();
	virtual void ImGuiDraw();
protected:

	vector<unique_ptr<IWindow>> windows;
};

