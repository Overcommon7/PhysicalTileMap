#include "pch.h"
#include "SaveAsModal.h"

#include "ImGuiUtilities/ImGuiUtils.h"

SaveAsModal::SaveAsModal()
	: IWindow("Save As")
	, mShouldSave(false)
{
	mIsClosable = true;
	mIsOpen = false;
	mIsPopup = true;
}

void SaveAsModal::ImGuiDrawBegin()
{
	if (mIsOpen)
		ImGui::OpenPopup(mTitle.c_str());
}

void SaveAsModal::ImGuiDrawEnd()
{
	
}

void SaveAsModal::ImGuiDraw()
{
	mShouldSave = false;
	if (!mIsOpen)
		return;
		
	if (ImGui::BeginPopupModal(mTitle.c_str(), &mIsOpen, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGuiUtils::SerializeString("Folder", mSavePath);

		if (fs::exists(mSavePath) && fs::is_directory(mSavePath))
		{
			if (ImGui::Button("Save"))
			{
				mShouldSave = true;
				Close();
				ImGui::CloseCurrentPopup();
			}
		}
		else
		{
			ImGui::TextDisabled("Save");
		}

		if (ImGui::Button("Cancel"))
		{
			Close();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
