#include "pch.h"
#include "DebugWindow.h"

#include "Game/Player/Player.h"

#include "Game/Physics/CollisionSolver.h"
#include "Game/Sprites/Enemy.h"

DebugWindow::~DebugWindow()
{
	mPlayer = nullptr;
	mCamera = nullptr;
	mCameraValues = nullptr;
	mEnemies = nullptr;
}

void DebugWindow::ImGuiDraw()
{
	CollisionSolver::ImGuiDraw();

	if (mPlayer)
		mPlayer->ImGuiDraw();

	if (mCameraValues)
		CameraMovement::ImGuiDraw(*mCamera, mCameraValues);

	if (mEnemies)
	{
		for (auto enemy : *mEnemies)
		{
			enemy->ImGuiDraw();
		}
	}
}
