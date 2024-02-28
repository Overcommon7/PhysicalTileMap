#include "pch.h"
#include "DebugWindow.h"

#include "Game/Player/Player.h"

#include "Game/Physics/CollisionSolver.h"

void DebugWindow::ImGuiDraw()
{
	CollisionSolver::ImGuiDraw();

	if (mPlayer)
		mPlayer->ImGuiDraw();
}
