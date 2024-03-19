#include "pch.h"
#include "OpenProjectModal.h"

#include "App/ILayer.h"
#include "ImGuiUtilities/ImGuiUtils.h"

OpenProjectModal::OpenProjectModal()
	: IWindow("Open Project")
{
	mIsClosable = true;
	mIsPopup = true;
	ILayer::SetCurrentPopup(this);
}

void OpenProjectModal::ImGuiDraw()
{
	bool interacted = ImGuiUtils::SerializeString("Project", mProjectPath);
	if (interacted) mCheckAgain = true;

	bool dir = false;
	fs::path path(fs::absolute(mProjectPath));

	if (interacted)
	{
		dir =
			fs::exists(path) &&
			fs::is_directory(path);

		if (dir)
		{
			for (auto file : fs::directory_iterator(path))
			{
				path = file.path();
				if (fs::is_regular_file(path) &&
					path.extension() == ".otp")
				{
					mDirPath = path;
					break;
				}
			}
		}
	}
	
	
	if (!mCheckAgain || 
		(mCheckAgain && 
		fs::exists(path) &&
		fs::is_regular_file(path) &&
		path.extension() == ".otp"))
	{
		mCheckAgain = false;
		if (ImGui::Button("Open"))
		{
			if (path.extension() == ".otp")
				OnProjectOpened.Invoke(path);
			else
				OnProjectOpened.Invoke(mDirPath);

				
			Close();
			ImGui::CloseCurrentPopup();
		}
	}
	else
	{
		ImGui::TextDisabled("Open");
	}
}
