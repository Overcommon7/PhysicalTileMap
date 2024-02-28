#pragma once
#include "ImGuiWindow/IWindow.h"

class Player;

class DebugWindow : public IWindow    
{
public:
	IWindowConstructor(DebugWindow) {}
	~DebugWindow() { mPlayer = nullptr; }
	void SetPlayer(Player* player) { mPlayer = player; }
	void ImGuiDraw() override;

	
private:
	Player* mPlayer = nullptr;
};

