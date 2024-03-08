#include "pch.h"
#include "DebugWindow.h"

#include "Game/Player/Player.h"

#include "Game/Physics/CollisionSolver.h"
#include "Camera/Camera.h"

void DebugWindow::ImGuiDraw()
{
	CollisionSolver::ImGuiDraw();

	if (mPlayer)
		mPlayer->ImGuiDraw();

	if (mCameraValues)
		CameraMovement::ImGuiDraw(*mCamera, mCameraValues);
}
