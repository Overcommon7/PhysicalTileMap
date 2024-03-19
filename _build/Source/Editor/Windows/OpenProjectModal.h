#pragma once
#include "ImGuiWindow/IWindow.h"

class OpenProjectModal : public IWindow
{
public:
	OpenProjectModal();
	void ImGuiDraw() override;

	Action<fs::path> OnProjectOpened;
private:	
	string mProjectPath = "Projects\\Test\\Test.otp";
	fs::path mDirPath;
	bool mCheckAgain = true;
};

